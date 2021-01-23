const ip="http://192.168.100.204"
const EXP="EXP001"
const fetch = require("node-fetch");
const admin= require('../firebase/firebaseConnection');
const fb_ops=require('../firebase/firebase_operations');
const database = admin.database();
const EXPRef = database.ref('/'+EXP);


EXPRef.on('value', snapshot => {
    var disp=0, disp=0
    var data = snapshot.val();
    if (data.Run==1){
     console.log(EXP+": Start")
     fb_ops.fetchip(ip,"/Start",EXP)
    }
    else if(data.Run==0){
      console.log(EXP+": Stop") 
      fb_ops.fetchip(ip,"/Stop",EXP)
    }
    else if(data.Run==3){
      console.log(EXP+": Para") 
      disp=data.Displacement
      length=data.Length  
      fb_ops.fetchip(ip,"/Para?A="+disp+"&B="+length,EXP)
    }
}); 
//....................................................................................



// app.get('/setdata', (req, res) => {
//   var data={
//       Data:30,
//       Run:2
//   }
//   usersRef.update(data,function(err){
//       if(err){
//           res.send(err);
//       }else{
//           res.json({"message":"Successfully updated data","result":true});
//       }
//   });

// });

module.exports =EXPRef;
//module.exports.EXPData=EXPData;
