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
- CPU: ARM64<br>
---
now go to `Project Settings > Player > Android` and do these settings:
- Scripting Backend: IL2CPP
- Target Architecture: ARM64<br>
---
after allat is done, you can finally use it in a unity c# script, like this:
```cs
using System.Runtime.InteropServices;
using UnityEngine;

public class FridaCheck : MonoBehaviour {
    [DllImport("FridaDetector")]
    private static extern bool IsFridaDetected();

    void Awake() {
        if (IsFridaDetected()) {
            Application.Quit();
        }
    }
}
```
you could make it send to a webhook, ban the player, etc. but in this example it just closes the app<br>
**(if you use this, make sure to never make the script name obvious. if you do they can just go into uabea and disable it)**<br>


## thats about it!
### if you have any other way of detecting frida please open a pr