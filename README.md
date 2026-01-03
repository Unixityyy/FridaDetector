# i do NOT have enough cpp knowledge to keep making this
# the current build doesnt work
---
#
---
# FridaDetector
since its mit license, you can do whatever you want but you have to give some credit

---
you need to add the `INTERNET` permission to ur `AndroidManifest.xml` so that it can scan for the frida port

## how to use
download `libFridaDetector.so` from the [releases](https://github.com/Unixityyy/FridaDetector/releases/tag/latest) tab, or build it urself<br>
(im not explaining how to build it urself)

---
drag the `libFridaDetector.so` into ur unity project at this path:<br>
`Assets/Plugins/Android/libs/arm64-v8a/libFridaDetector.so`<br>
when you do that, make sure its selected in unity, (the `libFridaDetector.so`) and in inspector select these settings:<br>
- Platform: Android
- CPU: ARM64
- Load on Startup: true<br>
---
now go to `Project Settings > Player > Android` and do these settings:
- Scripting Backend: IL2CPP
- Target Architecture: ARM64<br>
---
after allat is done, you can finally use it in a unity c# script, like this:
```cs
using System.Runtime.InteropServices;
using UnityEngine;
using System.Threading;
using UnityEngine.Scripting;

[System.Reflection.Obfuscation(Exclude = true)]
[Preserve]
public static class FridaCheck {
    [DllImport("web_utils", EntryPoint = "NativeCheck")]
    private static extern void NativeCheck();

    // if you get the DllNotFoundException even when the .so is in the apk, change SubsystemRegistration to BeforeSceneLoad
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
    static void Initialize() {
        // run in background to prevent it fucking with the main thread
        // when its on main thread it hangs
        Thread securityThread = new Thread(() => {
            try {
                NativeCheck();
            } catch (System.DllNotFoundException) {
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }
        });
        
        securityThread.IsBackground = true;
        securityThread.Start();
    }
}
```
you could make it send to a webhook, ban the player, etc. but in this example it just closes the app<br>
**(the approach above *should* make it invisible to uabea)**<br>
you could also make this check run like every 5 mins

---
### also renaming the lib is recommended. instead of `libweb_utils.so` (default name), you can make it `libOVRTracking.so`, or some legit sounding name. just make sure to update the `DllImport` **and** the cmakelists.txt if you do this!

## thats about it!
### if you have any other way of detecting frida please open a pr