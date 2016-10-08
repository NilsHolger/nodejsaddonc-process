var sunshine = require("./cpp/build/Release/sunshine");

var location = {
    latitude: 49.00, longitude: 8.40,
    samples : [
        { date: "2016-10-05", sunshine: 28},
        { date: "2016-10-06", sunshine: 30},
        { date: "2016-10-07", sunshine: 32},
        { date: "2016-10-08", sunshine: 34}
    ]
};
console.log(`Average sunshine = ${sunshine.avg_sunshine(location)} degrees celsius`); // 31

var data = sunshine.sunshine_data(location);
console.log(`Mean = ${data.mean}`);  //31
console.log(`Median = ${data.median}`); //31
console.log(`Standard Deviation = ${data.standard_deviation}`); //2.5819
console.log(`Count = ${data.n}`); //4

// console.log(`Is user authorized = ${sunshine.simple_login_sync("you sexy","string")}`);

// setInterval(function() {
//   console.log('Another operation');
// }, 500);

// sunshine.logInAsync("you sexy","code",function(data) {
//   console.log(data);
//   process.exit();
// });
// console.log('Async login started.')