#include <node.h>
#include <v8.h>
#include "sunshine.h"

using namespace v8;

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

location unpack_location(Isolate * isolate, const v8::FunctionCallbackInfo<v8::Value>& args){

        location loc;
        Handle<Object> location_obj = Handle<Object>::Cast(args[0]);
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

    location loc = unpack_location(isolate, args);
    double avg = avg_sunshine(loc);


    Local<Number> retVal = v8::Number::New(isolate, avg);
    args.GetReturnValue().Set(retVal);

}


void init(Handle<Object> exports, Handle<Object> module){
    //register function to make it callable from node here
    NODE_SET_METHOD(exports, "avg_sunshine", AvgSunshine);

}

//associate the module name with initialization logic
NODE_MODULE(sunshine, init)