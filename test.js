
function getRandom(max, min = 0) {
  return (Math.random() * (max - min) + min).toFixed(2);
}


for (var i = 0; i < 9; i++){
console.log(getRandom(60));
}