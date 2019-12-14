var canvas = new Canvas(1200, 400);
console.log("Type 'COMM.debug=1' to enable debugging");
//COMM.debug = 1;
serial.ilovebill=true;
_blocks = [];

class Analyzer
{
  constructor()
  {
    this.promise = null;
    this.promises = [];
    this.setup();
  }

  setup()
  {
    this.schedule( () =>
      Promise.resolve()
      // first request might be corrupted, just do not wait for result
      .then( () => new Promise( (resolve, reject) => { ANAL.Status(); setTimeout(resolve, 300);} ) )
      .then( () => ANAL.Configure() )     
      .then( () => ANAL.Restart() )
    );
  }

  loop()
  {
    if (!COMM.open || this.promise)
      return;

    if (this.promises.length > 0)
    {
      this.promise = this.promises.shift()().then( (() => this.promise = false).bind(this) );
      return;
    }

    this.schedule( () => Promise.resolve()
      .then( () => ANAL.Status() )
      .then( (ready) =>  
      {
        let FPGA_TRI = 0x10;
        ready = ready & FPGA_TRI;
        if (!ready)
          return;


        _blocks = [];

        return Promise.resolve()     // TODO: timeout, separate ANAL function for bulk transfer
          .then( () => ANAL.Get() )
          .then( (data) => _blocks.push(data) )
          .then( () => ANAL.Get() )
          .then( (data) => _blocks.push(data) )
          .then( () => ANAL.Get() )
          .then( (data) => _blocks.push(data) )
          .then( () => ANAL.Get() )
          .then( (data) => _blocks.push(data) )
          .then( () =>
          {
            return _blocks[0].concat(_blocks[1]).concat(_blocks[2]).concat(_blocks[3]);
          })
          .then((data) => canvas.RenderData(data))
          .then(() => ANAL.Restart());
      })
    );
  }

  start()
  {    
    setInterval( (()=>
    {
      this.loop();
    }).bind(this), 50);
  }

  schedule(p)
  {
    this.promises.push(p);
  }
}

var analyzer = new Analyzer();
analyzer.start();