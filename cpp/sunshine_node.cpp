#include <node.h>
#include <v8.h>
#include "sunshine.h"
#include <uv.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <windows.h>

using namespace v8;

void pack_sunshine_results(v8::Isolate* isolate, v8::Local<v8::Object> & target, sunshine_result & result);

sample unpack_sample(Isolate * isolate, const Handle<Object> sample_obj){

            sample s;
            Handle<Value> date_value = 
            sample_obj->Get(String::NewFromUtf8(isolate, "date"));
            Handle<Value> sunshine_value =
            sample_obj->Get(String::NewFromUtf8(isolate, "sunshine"));

            v8::String::Utf8Value utfValue(date_value);
            s.date = std::string(*utfValue);

            s.sunshine = sunshine_value->NumberValue();
            return s;
}

location unpack_location(Isolate * isolate, const Handle<Object> location_obj){
        location loc;
        Handle<Value> lat_value =
        location_obj->Get(String::NewFromUtf8(isolate, "latitude"));
        Handle<Value> lon_value =
        location_obj->Get(String::NewFromUtf8(isolate, "longitude"));

        loc.latitude = lat_value->NumberValue();
        loc.longitude = lon_value->NumberValue();

        Handle<Array> array = Handle<Array>::Cast(location_obj->Get(String::NewFromUtf8(isolate, "samples")));

        int sample_count = array->Length();

        for (int i = 0; i < sample_count; i++){
            sample s = unpack_sample(isolate, Handle<Object>::Cast(array->Get(i)));
            loc.samples.push_back(s);
        }
        return loc;
}

void AvgSunshine(const v8::FunctionCallbackInfo<v8::Value>& args){

    Isolate * isolate = args.GetIsolate();

    location loc = unpack_location(isolate, Handle<Object>::Cast(args[0]));
    double avg = avg_sunshine(loc);

    Local<Number> retVal = v8::Number::New(isolate, avg);
    args.GetReturnValue().Set(retVal);
}

void SimpleLoginSync(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate * isolate = args.GetIsolate();

    v8::String::Utf8Value param1(args[0]->ToString());
    std::string user = std::string(*param1);
    v8::String::Utf8Value param2(args[1]->ToString());
    std::string pwd = std::string(*param2); 
    //she has so many secrets
    double result = is_user_authorized(user, pwd);
    Local<Number> retVal = v8::Number::New(isolate, result);
    args.GetReturnValue().Set(retVal);
}

void pack_sunshine_results(v8::Isolate* isolate, v8::Local<v8::Object> & target, sunshine_result & result){
    target->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result.mean));
    target->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
    target->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
    target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));
}

void SunshineData(const v8::FunctionCallbackInfo<v8::Value>& args){
    Isolate * isolate = args.GetIsolate();

    location loc = unpack_location(isolate, Handle<Object>::Cast(args[0]));
    sunshine_result result = calc_sunshine_stats(loc);

    //create a new object on the V8 heap
    Local<Object> obj = Object::New(isolate);
    //transfer data from result to object
    obj->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result.mean));
    obj->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
    obj->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
    obj->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));

    //return the the object to JAVASCRIPT
    args.GetReturnValue().Set(obj);
}

void CalculateResults(const v8::FunctionCallbackInfo<v8::Value>&args){
    Isolate * isolate = args.GetIsolate();

    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();

    std::vector<location> locations;    //get this from node
    std::vector<sunshine_result> results; //get this from c++

    for (unsigned int i = 0; i < num_locations; i++){
        locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    };
    results.resize(locations.size());
    std::transform(locations.begin(), locations.end(), results.begin(), calc_sunshine_stats);
    
    //populate this with the results
    Local<Array> result_list = Array::New(isolate);

    for (unsigned int i = 0; i < results.size(); i++){
        Local<Object> result = Object::New(isolate);
        pack_sunshine_results(isolate, result, results[i]);
        result_list->Set(i, result);
    }
    //send it back to node as the return value
    args.GetReturnValue().Set(result_list);
}

struct Work {
    uv_work_t request;
    Persistent<Function> callback;
    std::vector<location> locations;
    std::vector<sunshine_result> results;
};

static void WorkAsync(uv_work_t *req){

    Work *work = static_cast<Work *>(req->data);
    work->results.resize(work->locations.size());
    std::transform(work->locations.begin(), work->locations.end(), work->results.begin(), calc_sunshine_stats);
    //she needs some sleep
    Sleep(3000);
}

static void WorkAsyncComplete(uv_work_t *req, int status){
    Isolate * isolate = Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    Work * work = static_cast<Work *>(req->data);
    //work has been done, pack results vector into local array on event threads stack
    Local<Array> result_list = Array::New(isolate);
    for (unsigned int i = 0; i < work->results.size(); i++){
        Local<Object> result = Object::New(isolate);
        pack_sunshine_results(isolate, result, work->results[i]);
        result_list->Set(i, result);
    }
    //set up return arguments
    Handle<Value> argv[] = { result_list };
    //execute the callback
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    //free up persistent function callback
    work->callback.Reset();
    delete work;
}



void CalculateResultsAsync(const v8::FunctionCallbackInfo<v8::Value>&args){
    Isolate * isolate = args.GetIsolate();
    Work * work = new Work();
    work->request.data = work;
    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();
    for (unsigned int i = 0; i < num_locations; i++){
        work->locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    }
    //store callback from JS in work package so it is callable
    Local<Function> callback = Local<Function>::Cast(args[1]);
    work->callback.Reset(isolate, callback);
    //kick of the worker thread to get the job done & invoke callback here
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);

    args.GetReturnValue().Set(Undefined(isolate));
}


void init(Handle<Object> exports, Handle<Object> module){
    //register functions to make them callable from node here
    NODE_SET_METHOD(exports, "avg_sunshine", AvgSunshine);
    NODE_SET_METHOD(exports, "simple_login_sync", SimpleLoginSync);
    NODE_SET_METHOD(exports, "sunshine_data", SunshineData);
    NODE_SET_METHOD(exports, "calculate_results", CalculateResults); //it's all about producing results: improve, improve, improve
    NODE_SET_METHOD(exports, "calculate_results_async", CalculateResultsAsync);
}

//associate the module name with initialization logic
NODE_MODULE(sunshine, init)