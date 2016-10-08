var sunshine = require("./cpp/build/Release/sunshine");

// var makeup = function(max){
//     return Math.round(max * Math.random() * 100 ) / 100;
// }
var random = function(max, min = 0){
      return (Math.random() * (max - min) + min).toFixed(2);
}

//build some dummy locations
var locations = [];

for (var i = 0; i < 10; i++){
    var loc = {
        latitude: random(180),
        longitude: random(180),
        samples : [
            {date: "2016-10-01", sunshine: random(40,30)},
            {date: "2016-10-02", sunshine: random(40,30)},
            {date: "2016-10-03", sunshine: random(40,30)},
            {date: "2016-10-04", sunshine: random(40,30)}
        ]
    }
    locations.push(loc);
}

//console.log(locations);

//invoke the addon, it's all about producing results and having fun in the process => ingenuity for life is our flag
var results = sunshine.calculate_results(locations);


//report the results from c++
var i = 0;
results.forEach((result) => {
    console.log(`Results for Location   ${i+1}`);
    console.log(`---------------------------`);
    console.log(`\tLatititude:         ${locations[i].latitude}`);
    console.log(`\tLongitude:          ${locations[i].longitude}`);
    console.log(`\tMean Sunshine:      ${result.mean.toFixed(2)} °C`);
    console.log(`\tMedian Sunshine:    ${result.median.toFixed(2)} °C`);
    console.log(`\tStandard Deviation: ${result.standard_deviation.toFixed(2)}°C`);
    console.log(`\tNumber of Samples:  ${result.n}`);
    console.log();
    i++;
})




// var location = {
//     latitude: 49.00, longitude: 8.40,
//     samples : [
//         { date: "2016-10-05", sunshine: 28},
//         { date: "2016-10-06", sunshine: 30},
//         { date: "2016-10-07", sunshine: 32},
//         { date: "2016-10-08", sunshine: 34}
//     ]
// };
// console.log(`Average sunshine = ${sunshine.avg_sunshine(location)} degrees celsius`); // 31

// var data = sunshine.sunshine_data(location);
// console.log(`Mean = ${data.mean}`);  //31
// console.log(`Median = ${data.median}`); //31
// console.log(`Standard Deviation = ${data.standard_deviation}`); //2.5819
// console.log(`Count = ${data.n}`); //4

// console.log(`Is user authorized = ${sunshine.simple_login_sync("you sexy","string")}`);

// setInterval(function() {
//   console.log('Another operation');
// }, 500);

// sunshine.logInAsync("you sexy","code",function(data) {
//   console.log(data);
//   process.exit();
// });
// console.log('Async login started.')