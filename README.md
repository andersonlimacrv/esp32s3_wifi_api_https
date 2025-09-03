# HTTPS-ESP-HTTPS-Server-Client

> ### Creating PEM Certificates for the Server

`Step 1`: To create the PEM certificates for the server side, use the following OpenSSL command:

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

Copy the displayed certificates and paste them into a single file ClientCert.pem for the client.

`Copy only what is between -----BEGIN CERTIFICATE----- (...) and -----END CERTIFICATE-----, which will appear 3 times.`

`Step 3`: Configuring the ESP32S3_01 Client
Now, you can configure your ESP32S3_01 client to connect to the site https://www.actions-fastapi.onrender.com using the generated PEM certificates.

`Step 4`: Adding Configuration FLAG in platformio.ini
In the configuration file to use platformio, and for the certificate path to be read, the FLAG should be included:

<!-- code .ini -->

```js
board_build.embed_txtfiles = 
    lib/utils/http_client/certs/ClientCert.pem
```

# Filling in the Credentials in the `Credentials.h` File

> ### Instructions on how to fill in the Wi-Fi credentials, login, and configure the equipment IDs in the `Credentials.h` file. This file will be created in the `src/` directory.

### Step 1: Create the `Credentials.h` File

1. Navigate to the `src/` directory in your project.
2. Create a new file called `Credentials.h`.

### Step 2: Fill in the Wi-Fi and Login Credentials

Open the `Credentials.h` file and add the following code:

```c
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "../lib/classes/IdsList/ID_List.h"
#define USE_EXTERNAL_BACKEND 0 // 0 = LOCAL; 1 = EXTERNAL CLOUD SGEM API

static const char * const CREDENTIALS_SSID = "YOUR_WIFI_SSID";              // WIFI SSID 
static const char * const CREDENTIALS_PASSWORD = "YOUR_WIFI_PASSWORD";      // WIFI PASSWORD 
static const char * const CREDENTIALS_LOGIN_USERNAME = "LOGIN_USERNAME";    // USERNAME FROM SGEM USER TO POST 
static const char * const CREDENTIALS_LOGIN_PASSWORD = "LOGIN_PASSWORD";    // PASSWORD FROM SGEM USER TO POST

// Function to initialize the equipment IDs inline, example with ID's
inline void initializeEquipmentIds() {
    Equipment& eq = Singleton<Equipment>::instance();
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
    #define CREDENTIALS_BACKEND_URL "http://192.168.2.155:7022" // Backend LOCAL from development environment
#endif

#endif  /* CREDENTIALS_H */
```

### Step 3: Configuring the Equipment IDs

The `initializeEquipmentIds` function is used to initialize the equipment IDs. This code is already included in the code block above.

### Step 4: Defining the Backend URL

In the provided code, the backend URL is conditionally defined based on the definition of `USE_EXTERNAL_BACKEND`.

- If `USE_EXTERNAL_BACKEND` is defined, the URL will be `https://actions-fastapi.onrender.com` (site backend).
- Otherwise, the default URL will be `http://192.168.2.155:7022` (localhost).

### Step 5: Saving the File

After inserting the code, save the `Credentials.h` file in the `src/` directory.

## Conclusion

You have successfully configured the `Credentials.h` file with the Wi-Fi credentials, login, equipment IDs, and backend URL. This file is essential for the correct operation of the HTTPS client on the ESP32.

`This tutorial covers all the necessary steps to correctly fill in and configure the 'Credentials.h' file and how to generate the certificate for a specific site when making HTTPS requests via SSL/TLS.`