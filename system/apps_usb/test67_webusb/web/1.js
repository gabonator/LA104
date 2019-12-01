dfu.Device.prototype.readConfigurationDescriptor = function(index) {
        const GET_DESCRIPTOR = 0x06;
        const DT_CONFIGURATION = 0x02;
        const wValue = ((DT_CONFIGURATION << 8) | index);

        return this.device_.controlTransferIn({
            "requestType": "standard",
            "recipient": "device",
            "request": GET_DESCRIPTOR,
            "value": wValue,
            "index": 0
        }, 4).then(
            result => {
                if (result.status == "ok") {
                    // Read out length of the configuration descriptor
                    let wLength = result.data.getUint16(2, true);
                    return this.device_.controlTransferIn({
                        "requestType": "standard",
                        "recipient": "device",
                        "request": GET_DESCRIPTOR,
                        "value": wValue,
                        "index": 0
                    }, wLength);
                } else {
                    return Promise.reject(result.status);
                }
            }
        ).then(
            result => {
                if (result.status == "ok") {
                    return Promise.resolve(result.data);
                } else {
                    return Promise.reject(result.status);
                }
            }
        );
    };


    dfu.Device.prototype.readStringDescriptor = async function(index, langID) {
        if (typeof langID === 'undefined') {
            langID = 0;
        }

        const GET_DESCRIPTOR = 0x06;
        const DT_STRING = 0x03;
        const wValue = (DT_STRING << 8) | index;

        const request_setup = {
            "requestType": "standard",
            "recipient": "device",
            "request": GET_DESCRIPTOR,
            "value": wValue,
            "index": langID
        }

        // Read enough for bLength
        var result = await this.device_.controlTransferIn(request_setup, 1);

        if (result.status == "ok") {
            // Retrieve the full descriptor
            const bLength = result.data.getUint8(0);
            result = await this.device_.controlTransferIn(request_setup, bLength);
            if (result.status == "ok") {
                const len = (bLength-2) / 2;
                let u16_words = [];
                for (let i=0; i < len; i++) {
                    u16_words.push(result.data.getUint16(2+i*2, true));
                }
                if (langID == 0) {
                    // Return the langID array
                    return u16_words;
                } else {
                    // Decode from UCS-2 into a string
                    return String.fromCharCode.apply(String, u16_words);
                }
            }
        }
        
        throw `Failed to read string descriptor ${index}: ${result.status}`;
    };
