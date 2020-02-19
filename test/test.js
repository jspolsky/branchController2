const dgram = require('dgram');


function sleep(milliseconds) {
  const date = Date.now();
  let currentDate = null;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}




console.log("ready");

var some_pixels = [
    [255, 255, 255],
    [0, 0, 0],
    [255, 0, 0],
    [0, 255, 0],
    [0, 0, 255],
];

var pixels = [];

for (var j = 0; j < 60; j++)
{
  pixels = pixels.concat(some_pixels);
}

console.log(pixels.length);

var num_pixels = 300;
var strip_id = 1;

var lengthBuffer = Buffer.alloc(2);
lengthBuffer[0] = parseInt((num_pixels * 3) / 256, 10);
lengthBuffer[1] = (num_pixels * 3) % 256;

const date = Date.now();
console.log('starting at ');
console.log(date);

for (var l = 0; l < 1200; l++)
{
  for (var k = 1; k <= 8; k++)
  {

    var header = Buffer.alloc(4);
    header[0] = k;
    header[1] = 0;
    header[2] = lengthBuffer.readUInt8(0);
    header[3] = lengthBuffer.readUInt8(1);

    var message_pixels = new Uint8Array( num_pixels * 3 );

    for(var i = 0; i < num_pixels; i++){
        if(pixels[i]){
          message_pixels[i*3] = pixels[i][0];
          message_pixels[(i*3)+1] = pixels[i][1];
          message_pixels[(i*3)+2] = pixels[i][2];
        }
      }

      var message;
      message = header;
      message = Buffer.concat([message, Buffer.from(message_pixels)]);

//    console.log("finished making a message");

    const client = dgram.createSocket('udp4');
    client.send(message, 7890, '192.168.1.7', (err) => { client.close(); });

 //   console.log("sent");

  }
}

console.log('finish at ');
console.log(Date.now());
