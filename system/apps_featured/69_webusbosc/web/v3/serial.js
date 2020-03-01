var serial = {};

  serial.getPorts = function() {
    return navigator.usb.getDevices().then(devices => {
      return devices.map(device => new serial.Port(device, this.ilovebill));
    });
  };

  serial.requestPort = function() {
    const filters = [
      { 'vendorId': 0x1209, 'productId': 0xdb42 }
    ];
    return navigator.usb.requestDevice({ 'filters': filters }).then(
      device => new serial.Port(device, this.ilovebill)
    );
  }

  serial.Port = function(device, ilovebill) {
    this.device_ = device;
    this.interfaceNumber_ = 3;  // original interface number of WebUSB Arduino demo
    this.endpointIn_ = 4;       // original in endpoint ID of WebUSB Arduino demo
    this.endpointOut_ = 3;      // original out endpoint ID of WebUSB Arduino demo
    this.ilovebill = ilovebill;
  };

  serial.Port.prototype.connect = function() {
    let readLoop = () => {
      this.device_.transferIn(this.endpointIn_, 1024*64).then(result => {
        this.onReceive(result.data);
        if (this.ilovebill)
          setTimeout(readLoop, 0); 
        else
          readLoop();
      }, error => {
        this.onReceiveError(error);
      });
    };

    return this.device_.open()
        .then(() => {
          if (this.device_.configuration === null) {
            return this.device_.selectConfiguration(1);
          }
        })
        .then(() => {

          var configurationInterfaces = this.device_.configuration.interfaces;
          configurationInterfaces.forEach(element => {
            element.alternates.forEach(elementalt => {
              if (elementalt.interfaceClass==10) {
                this.interfaceNumber_ = element.interfaceNumber;
                elementalt.endpoints.forEach(elementendpoint => {
                  if (elementendpoint.direction == "out") {
                    this.endpointOut_ = elementendpoint.endpointNumber;
                  }
                  if (elementendpoint.direction=="in") {
                    this.endpointIn_ =elementendpoint.endpointNumber;
                  }
                })
              }
            })
          })

        })
        .then(() => this.device_.claimInterface(this.interfaceNumber_))
        .then(() => this.device_.selectAlternateInterface(this.interfaceNumber_, 0))

        .then(() => this.device_.controlTransferOut({
            'requestType': 'class',
            'recipient': 'interface',
            'request': 0x22,
            'value': 0x01,
            'index': this.interfaceNumber_}))

        .then(() => {
          readLoop();
        });
  };

  serial.Port.prototype.disconnect = function() {
    return this.device_.controlTransferOut({
            'requestType': 'class',
            'recipient': 'interface',
            'request': 0x22,
            'value': 0x00,
            'index': this.interfaceNumber_})
        .then(() => this.device_.close());
  };

  serial.Port.prototype.send = function(data) {
    return this.device_.transferOut(this.endpointOut_, data);
  };
