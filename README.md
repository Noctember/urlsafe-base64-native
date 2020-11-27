# 

Designed for usability and speed. No padding.

Alphabets as described in [RFC 4648](https://tools.ietf.org/html/rfc4648).


## API

```js
const Base64 = require('urlsafe-base64-native');
```

```ts
Base64.encode(input: String | input: Buffer): String
```

```ts
Base64.decode(input: String | input: Buffer): String
```

## Why?

All packages I found on npm encode into url-safe base64 using Buffer.toString and then replacing +/ with -_. That's ugly.


## Credits
- [Ravy](https://ravy.dev/root/b64u)
