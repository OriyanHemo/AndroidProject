Java.perform(function () {
    var log = Java.use("android.util.Log");
    log.i("FridaScript", "GREAT JOB!");
});
//frida-ps -U
//frida -U -l ./myscript.js -p 13217