#include <napi.h>

namespace base64addon {
    Napi::String Encode(const Napi::CallbackInfo& info);
    Napi::String Decode(const Napi::CallbackInfo& info);
    Napi::Object Init(Napi::Env env, Napi::Object exports);
}



