## Challenge-Response Protocol using a Digital Signature

### Run locally
For compilation, the miracl library has been used. The compiled `.a` file and header files have been included but in case they don't work for your system. [Please follow this guide to install miracl yourself.](https://adityagudimetla.medium.com/installing-miracl-for-dummies-7eb7192c3285)

```bash
gcc genkey.c -o genkey -I ./include libmiracl.a
./genkey    // to generate keys
gcc sign.c -o sign -I ./include libmiracl.a
./sign      // to generate digital signature
gcc server.c -o server -I ./include libmiracl.a
gcc client.c -o client -I ./include libmiracl.a
```
Then run `./server` and `./client` on two separate terminals.

### Walkthrough

We have implemented a client-server setup where the server has access to the public key of the client while the client owns the corresponding private key.

First part of the code is `genkey.c` which generates the public-private key pair.

![](https://i.imgur.com/WKWOSoc.png)

These keys are stored in files `public.key` and `private.key` for later use. Server is given access to only the public key, while Client has access to both.

Now, the next part is the digital signature itself for the client side. The `sign.c` part takes care of that. It extracts the private key from the file, and signs the data of a file using it, and fetches out the signature.

![](https://i.imgur.com/Mea4Rfs.png)

10FB8E is the digital signature of the file generated using the client's private key.

Now we start running our server and the client attempts to connect to it.

Server Process:

![](https://i.imgur.com/uUtNCFt.png)

Client Process:

![](https://i.imgur.com/EZBafFc.png)

Now, the client types 'connect' to start the authentication process and the server asks it for the digital signature as the **challenge** part of the Challenge-Response Protocol.
The client then sends it the digital signature it generated earlier as the **response** part of the Challenge-Response Protocol.

Correct Signature Case:

![](https://i.imgur.com/WSNE6yR.png)

Wrong Signature Case:

![](https://i.imgur.com/FiquqAU.png)

The server uses the client's public key to verify the signature sent by the client.
