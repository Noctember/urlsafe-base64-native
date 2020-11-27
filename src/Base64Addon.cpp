#include "Base64Addon.hpp"

const char base64_url_alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
};

Napi::String base64addon::Encode(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !(info[0].IsBuffer() || info[0].IsString())) {
        Napi::TypeError::New(env, "String or Buffer expected").ThrowAsJavaScriptException();
    }

    std::string input;
    if (info[0].IsBuffer()) {
        input = info[0].As<Napi::Buffer<Napi::String>>().ToString().As<Napi::String>();
    } else {
        input = info[0].As<Napi::String>();
    }

    std::string out;
    int val = 0;
    int valb = -6;
    for (int i = 0; i < input.length(); i++) {
        unsigned char c = input[i];
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_url_alphabet[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        out.push_back(base64_url_alphabet[((val<<8)>>(valb+8))&0x3F]);
    }

    return Napi::String::New(env, out);
}

Napi::String base64addon::Decode(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !(info[0].IsBuffer() || info[0].IsString())) {
        Napi::TypeError::New(env, "String or Buffer expected").ThrowAsJavaScriptException();
    }
    std::string input;

    if (info[0].IsBuffer()) {
        input = info[0].As<Napi::Buffer<Napi::String>>().ToString().As<Napi::String>();
    } else {
        input = info[0].As<Napi::String>();
    }

    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_url_alphabet[i]] = i;

    int val = 0;
    int valb = -8;
    for (int i = 0; i < input.length(); i++) {
        unsigned char c = input[i];
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return Napi::String::New(env, out);
}


Napi::Object base64addon::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("encode", Napi::Function::New(env, base64encoder::Encode));
    exports.Set("decode", Napi::Function::New(env, base64encoder::Decode));

    return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    base64encoder::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll);
