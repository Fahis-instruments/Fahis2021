const admin= require('../firebase/firebaseConnection');
const database = admin.database();
const EXPRef = database.ref('/EXP001');
const NodeMediaServer = require('node-media-server');
const ngrok = require('ngrok');

ClearAllLinks=async()=>{
 await ngrok.disconnect(); // stops all
 console.log("All Links stopped")
}

async function GenerateLink(){
  try{
  const url = await ngrok.connect({
    addr: 8001,
    onStatusChange: status => {console.log(status)}, // 'closed' - connection is lost, 'connected' - reconnected
  });
  const data = {URL:url+"/live/test_stream.flv",Run:9}
  console.log(url)
  await EXPRef.update(data,function(err){
    if(err){
        throw new Error(`Video Link not Updated: ${err}`);
    }
    else{
      console.log({"message":"Successfully updated Video Link","result":true});
    }
  }); 
  }
  catch(e){
    //VideoStream.ClearAllLinks();
    console.log(e);
  }  
};


StartStream = ()=>{ 
const config = {
  rtmp: {
    port: 1935,
    chunk_size: 60000,
    gop_cache: true,
    ping: 30,
    ping_timeout: 60
  },
  http: {
    port: 8001,
    allow_origin: '*'
  }
};

var nms = new NodeMediaServer(config)
nms.run();
}

module.exports={
    StartStream,
    GenerateLink,
    ClearAllLinks    
}