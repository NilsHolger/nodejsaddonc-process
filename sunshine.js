var sunshine = require("./cpp/build/Release/sunshineasync");

// var location = {
//     latitude: 49.00, longitude: 8.40,
//     samples : [
//         { date: "2016-10-05", sunshine: 27.5},
//         { date: "2016-10-06", sunshine: 30.125}
//     ]
// };
// console.log(`Average sunshine = ${sunshine.avg_sunshine(location)} degrees celsius`);

// console.log(`Is user authorized = ${sunshine.simple_login_sync("you sexy","string")}`);

setInterval(function() {
  console.log('Another operation');
}, 500);

sunshine.logInAsync("you sexy","code",function(data) {
  console.log(data);
  process.exit();
});
console.log('Async login started.')