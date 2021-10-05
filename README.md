## Challenge-Response Protocol using a Digital Signature

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
