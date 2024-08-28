# HTTPS-ESP-HTTPS-Server-Client

> ### Creating PEM Certificates for the Server

`Step 1`: To create PEM certificates for the server side, use the following OpenSSL command:

```bash
openssl req -newkey rsa:2048 -nodes -keyout ServerKey.pem -x509 -days 3650 -out ServerCert.pem -subj "/CN=ESP32S3_01_HTTPS"
```

Replace ESP32S3_01 HTTPS server with your server name, if desired.

`Step 2`: Creating PEM Certificates for the Client
Use the following command to view the certificates of the site you want to connect to, such as https://actions-fastapi.onrender.com:

```bash
openssl s_client -showcerts -connect actions-fastapi.onrender.com:443
```

_NOTE_: `The website should not contain www and will always be on port 443.`

Copy the displayed certificates and paste them into a single file named ClientCert.pem for the client.

`Copy only what is between -----BEGIN CERTIFICATE----- (...) and -----END CERTIFICATE-----, which will appear 3 times.`

`Step 3`: Configuring the ESP32S3_01 Client
Now, you can configure your ESP32S3_01 client to connect to the site https://www.actions-fastapi.onrender.com using the generated PEM certificates.

# Filling in the Credentials in the `Credentials.h` File

> ### Guidelines on how to fill in Wi-Fi credentials, login, and configure equipment IDs in the `Credentials.h` file. This file will be created in the `src/` directory.

### Step 1: Create the `Credentials.h` File

1. Navigate to the `src/` directory in your project.
2. Create a new file named `Credentials.h`.

### Step 2: Fill in the Wi-Fi and Login Credentials

Open the `Credentials.h` file and add the following code:

```c

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "../lib/classes/IdsList/ID_List.h"
#define USE_EXTERNAL_BACKEND 1

static const char * const CREDENTIALS_SSID = "YOUR_WIFI_SSID";
static const char * const CREDENTIALS_PASSWORD = "YOUR_WIFI_PASSWORD";
static const char * const CREDENTIALS_LOGIN_USERNAME = "LOGIN_USERNAME";
static const char * const CREDENTIALS_LOGIN_PASSWORD = "LOGIN_PASSWORD";

// Function to initialize the equipment IDs inline, example with IDs
inline void initializeEquipmentIds() {
    IdsList& eq = Singleton<IdsList>::instance();
    eq.condensadorId = 1;
    eq.condensacaoId = 4;
    eq.bombaIds = {1, 2};
    eq.ventiladorIds = {1, 2};
    eq.ambienteIds = {1, 2, 3};
    eq.compressorIds = {1};
    eq.regimeIds = {1, 2, 3};
}

// Definition of the backend URL
#ifdef USE_EXTERNAL_BACKEND
    #define CREDENTIALS_BACKEND_URL "https://actions-fastapi.onrender.com" // Example backend URL
#else
    //#define CREDENTIALS_BACKEND_URL "http://192.168.18.18:7022"
    #define CREDENTIALS_BACKEND_URL "http://192.168.2.155:7022"
#endif

#endif  /* CREDENTIALS_H */
```

### Step 3: Configuring the Equipment IDs

The `initializeEquipmentIds` function is used to initialize the equipment IDs. This code is already included in the block of code above.

### Step 4: Defining the Backend URL

In the provided code, the backend URL is defined conditionally based on the `USE_EXTERNAL_BACKEND` definition.

- If `USE_EXTERNAL_BACKEND` is defined, the URL will be `https://actions-fastapi.onrender.com` (site backend).
- Otherwise, the default URL will be `http://192.168.2.155:7022` (localhost).

### Step 5: Saving the File

After entering the code, save the `Credentials.h` file in the `src/` directory.

## Conclusion

You have successfully configured the `Credentials.h` file with Wi-Fi credentials, login, equipment IDs, and backend URL. This file is essential for the correct operation of the HTTPS client on the ESP32.

`This tutorial covers all the necessary steps to fill in and configure the 'Credentials.h' file correctly and how to generate the certificate for a specific site when making HTTPS requests via SSL/TLS.`
