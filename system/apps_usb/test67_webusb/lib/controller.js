const util = require('util')
var usb = require('usb');
// DMX512 PC Computer USB Satge Lighting Controller Dimmer to DMX Interface Adapter
// Bus 020 Device 010: ID 16c0:05dc 16c0 uDMX  Serial: ilLUTZminator001

//0x1209, 'productId': 0xdb42

//var dev = usb.findByIds(0x1209, 0xdb42);
var dev = usb.findByIds(0x0483, 0x5740);
//dev.open();                                // open device
dev.__open();
dev.__claimInterface(1);

//device.interfaces[0].claim();
//dev.interfaces[1].claim();
//console.log(device.interfaces[1].address);
//dev.__open();
//dev.__claimInterface(1);
//console.log(dev.interfaces[0].isKernelDriverActive())

console.log(dev);
var ep = dev.interfaces[1].endpoints[0];

setTimeout( () =>
{
  var buffer = new Buffer(0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 );
  dev.controlTransfer(0x21, 0x20, 0, 0, buffer, (error, data) => {
    console.log(error);
  })

  setInterval( () =>
  {
    ep.transfer(new Buffer(0x31, 0x32), console.log);
  }, 1000);

}, 1000);



return;

console.log(device.interfaces[0].endpoints.length);
console.log(device.interfaces[1].endpoints.length);

{
//var e =1;
//console.log([i, e]);
//console.log (device.interfaces[i].endpoints.length)
//device.interfaces[i].endpoints[e].transfer(new Buffer(0x31, 0x32), console.log); // start polling the USB for data event to fire
}
var ep = device.interfaces[1].endpoints[0];
console.log(ep.address);
console.log(ep.direction);
console.log(ep.transferType);
//console.log(usb.LIBUSB_TRANSFER_TYPE_BULK);
setInterval( () =>
{
  ep.transfer(new Buffer(0x31, 0x32), console.log);
}, 100);
//device.interfaces[1].endpoints[0].end();

return;
dev.__open();
dev.__claimInterface(1);
dev.open();
//console.log(dev.interfaces));
//console.log(util.inspect(dev.interfaces, {showHidden: false, depth: null}))

var i = 0;

setInterval(() =>
{
var cSetMultiChannel = i % 8;
var address = Math.floor(i / 8);
i++;
console.log([cSetMultiChannel, address]);

  var cRequestType = usb.LIBUSB_REQUEST_TYPE_VENDOR | usb.LIBUSB_RECIPIENT_DEVICE | usb.LIBUSB_ENDPOINT_OUT;

  var buffer = new Buffer(0x32, 0x33, 0x34);
  dev.controlTransfer(cRequestType, cSetMultiChannel, buffer.length, address, buffer, (error, data)=>{
    console.log(error);
  })
}, 200);