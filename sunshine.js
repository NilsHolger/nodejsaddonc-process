var sunshine = require("./cpp/build/Release/sunshine");

var location = {
    latitude: 49.00, longitude: 8.40,
    samples : [
        { date: "2016-10-05", sunshine: 27.5},
        { date: "2016-10-06", sunshine: 30.125}
    ]
};
console.log(`Average sunshine = ${sunshine.avg_sunshine(location)} degrees celsius`);