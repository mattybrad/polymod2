const { Client, Server } = require('node-osc');

const client = new Client('127.0.0.1', 3333);
var server = new Server(3333, '0.0.0.0');

server.on('listening', () => {
  console.log('OSC Server is listening.');
})

server.on('message', (msg) => {
  console.log(`Message: ${msg}`);
  //server.close();
});

setInterval(function(){
  client.send('/hello', 'world', (err) => {
    if (err) console.error(err);
    //client.close();
  });
}, 1000);
