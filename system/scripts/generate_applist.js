const { promises: fs } = require("fs");
var path = require('path')
const fs2 = require("fs");
//import {convert} from 'imagemagick-convert';
const convert = require("imagemagick-convert");
var crypto = require('crypto');
//const {promises : convert} = require('imagemagick-convert');

async function getFiles(base = "./", ext = ".lnk", path = "") {
    const entries = await fs.readdir(base + path, { withFileTypes: true });

    // Get files within the current directory and add a path key to the file objects
    const files = entries
        .filter(file => !file.isDirectory())
        .filter(file => file.name.substr(-ext.length) == ext && file.name != "index.lnk" && file.name != "help.lnk")
        .map(file => ({ base:base, path:path, /*...file,*/ name:file.name }));

    // Get folders within the current directory
    const folders = entries.filter(folder => folder.isDirectory());

    for (const folder of folders)
        /*
          Add the found files within the subdirectory to the files array by calling the
          current function itself
        */
        files.push(...await getFiles(base, ext, `${path}${folder.name}/`));

    return files;
}

var root = "../build/image_la104/";
var appHashes = [];
var appList = [];
var appIds = {};

  function getFileHash(name)
  {    
    return new Promise((resolve, reject) =>
    {
      var checksum = (str, algorithm) => {
         return crypto
           .createHash(algorithm)
           .update(str, 'binary')
           .digest('hex')
      }
      fs2.readFile(name, function (err, data) {
        resolve(checksum(data, 'sha1'));
      })
    });
  }

// trace directory structure, assign path to every application id (unique number)
getFiles("../", "build.sh").then(l => l.filter(app => app.path.substr(0,4) == "apps").map(app => app.path))
.then(l => {
  for (var i in l)
  {
    var folder = l[i].split("/")[1];
    folder = folder.replace("test", "")
    folder = folder.split("_")[0];
    appIds[folder] = l[i];
  }
})
// calculate hashes of all built ELF apps
.then( () => getFiles("../build/apps/", ".elf")).then(flist =>
{
  flist = flist.map(f => f.base + f.path + f.name);
  var promises = [];
  var addHash = (name) =>
  {
//console.log("add: " + name);
    var id = parseInt(name.split("/")[3])
    return getFileHash(name).then((hash) => appHashes[hash] = {internal:name, id:id, folder:appIds[id]});
  };

  for (var i in flist)
    promises.push(addHash(flist[i]));
  return Promise.all(promises);
})
.then(() => getFiles("../build/image_la104/", ".lnk"))
.then(flist =>
{
  var final = [];
  for (f in flist)
  {
    var lnk = fs2.readFileSync(flist[f].base + flist[f].path + flist[f].name).toString();
    json = eval("_="+lnk);
    final.push({desc:json.description.long, path:flist[f].path, icon:json.icon, execute:json.execute, youtube:json.youtube ? json.youtube : null});
  }
  appList = final;
  return final;
}).then(() =>
{
  var root = "../build/image_la104/";
  var promises = [];
  for (f in appList)
  {
    ((f) => 
    {
      var execf = root + appList[f].path + appList[f].execute;
      promises.push(getFileHash(execf).then((hash) => appList[f].hash = hash));
    })(f);
  }
  return Promise.all(promises);
})
.then(() =>
{ 
  var root = "../build/image_la104/";
  var promises = [];
  for (var i=0; i<appList.length; i++)
  {
    var imgsrc = root + appList[i].path + appList[i].icon;
    var target = appList[i].icon;
    target = target.split("/").join("_");
    target = target.replace(".bmp", ".png");
    var group = appList[i].path.split("/");
    group.pop();
    group = group.join("/");
    appList[i].group = group;
    appList[i].newIcon = target;
//    console.log("| " + target + " | " + group + " | " + appList[i].desc + " |");
//console.log([imgsrc, target]);
    var conv = (imgsrc, target) =>
    {
      return convert.convert({
        srcData: fs2.readFileSync(imgsrc),
  //      srcFormat: 'BMP',
//        width: 64,
//        height: 64,
    //    resize: 'crop',
        format: 'PNG',
      }).then(imgBuffer =>
      {
//        console.log("Write: " + target);
        fs2.writeFileSync(target, imgBuffer);
      }).catch(e => {
      });
    };
    promises.push(conv(imgsrc, "../build/appicons/"+target));
  }
  return Promise.all(promises);
})
.then(() => {
  console.log("| Icon | Group | App name | App project / video |");
  console.log("|------|-------|----------|---------------------|");

  for (var i=0; i<appList.length; i++)
  {
    var app = appList[i];
    var ref = appHashes[app.hash];
    if (!ref)
    {
      throw "Cannot match app: " + JSON.stringify(app);
    }
    var icon = "!["+ref.internal.split("/").slice(-1)[0]+"](resources/appicons/"+app.newIcon+")";
    var folder = "["+ref.folder.slice(0,-1)+"](system/"+ref.folder+")";
    if (app.youtube)
      folder += "<br\>[https://youtu.be/"+app.youtube+"](https://youtu.be/"+app.youtube+")";
    console.log("| " + icon + " | " + app.group + " | " + app.desc + " | " + folder + " |");
  }
});
