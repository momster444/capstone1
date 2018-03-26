const express = require('express')
const app = express()
const fs = require('fs')

var seq = 0
app.get('/update', function(req, res){
       fs.appendFile('log.txt',JSON.stringify(req.query)+"\n", function(err){
           if(err) throw err
           res.end("Got "+String(seq++)+" "+JSON.stringify(req.query))
       });
})
app.get('/get', function (req, res) {
        var ans = ""
        var array = fs.readFileSync('log.txt').toString().split("\n")

        for(var i=0;i<array.length;i++)
	{
		ans = ans + array[i]+"<br>"
		console.log(array[i])
        }
        res.send(ans) 
})
app.listen(3000, ()=>console.log('listening on port 3000!'))
