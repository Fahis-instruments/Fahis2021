const express = require('express');
const EXPRef001=require('./Experiments/EXP001');
const EXPRef002=require('./Experiments/EXP002');
const VideoStream=require('../Fahis2021-main/EXPVideoStream/EXP001Stream')
const app = express();
const port = 3000;

app.use(express.json());
app.use(express.static('public'));
VideoStream.StartStream();
app.get('/', (req, res) => {
    res.send('index');
});
app.get('/Start', (req, res) => {
  console.log("Request Received")
  
  res.send('index');
});

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

app.listen(port, () => {
  console.log(`App is listening to port ${port}`);
});