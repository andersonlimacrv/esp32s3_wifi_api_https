# HTTPS-ESP-HTTPS-Server-Client

> ### Criando Certificados PEM para o Servidor

`Passo 1`: Para criar os certificados PEM para o lado do servidor, utilize o seguinte comando OpenSSL:

```bash
openssl req -newkey rsa:2048 -nodes -keyout ServerKey.pem -x509 -days 3650 -out ServerCert.pem -subj "/CN=ESP32S3_01_HTTPS"
```

Substitua ESP32S3_01 HTTPS server pelo nome do seu servidor, se desejar.

`Passo 2`: Criando Certificados PEM para o Cliente
Utilize o comando a seguir para visualizar os certificados do site ao qual deseja se conectar, como por exemplo, https://actions-fastapi.onrender.com:

```bash
openssl s_client -showcerts -connect actions-fastapi.onrender.com:443
```

_OBS_: `O website não deverá conter www e sempre será na porta 443.`

Copie os certificados exibidos e cole-os em um único arquivo ClientCert.pem para o cliente.

`Copie apenas oque está entre -----BEGIN CERTIFICATE----- (...) e -----END CERTIFICATE-----, que aparecerão 3 vezes.`

`Passo 3`: Configurando o Cliente ESP32S3_01
Agora, você pode configurar o seu cliente ESP32S3_01 para se conectar ao site https://www.actions-fastapi.onrender.com utilizando os certificados PEM gerados.

`Passo 4`: Adicionando FLAG de configuração no plataformio.ini
No arquivo de configuração para utilizar o platformio, e que o caminho do certificado seja lido, deverá ser incluída a FLAG:

<!-- codigo .ini -->

```js
board_build.embed_txtfiles = 
    lib/utils/http_client/certs/ClientCert.pem
```

# Preenchendo as Credenciais no Arquivo `Credentials.h`

> ### Orientações de como preencher as credenciais de Wi-Fi, login e configurar os IDs de equipamentos no arquivo `Credentials.h`. Esse arquivo será criado no diretório `src/`.

### Passo 1: Criar o Arquivo `Credentials.h`

1. Navegue até o diretório `src/` no seu projeto.
2. Crie um novo arquivo chamado `Credentials.h`.

### Passo 2: Preencher as Credenciais de Wi-Fi e Login

Abra o arquivo `Credentials.h` e adicione o seguinte código:

```c
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "../lib/classes/IdsList/ID_List.h"
#define USE_EXTERNAL_BACKEND 0 // 0 = LOCAL; 1 = EXTERNAL CLOUD SGEM API

static const char * const CREDENTIALS_SSID = "YOUR_WIFI_SSID";              // WIFI SSID 
static const char * const CREDENTIALS_PASSWORD = "YOUR_WIFI_PASSWORD";      // WIFI PASSWORD 
static const char * const CREDENTIALS_LOGIN_USERNAME = "LOGIN_USERNAME";    // USERNAME FROM SGEM USER TO POST 
static const char * const CREDENTIALS_LOGIN_PASSWORD = "LOGIN_PASSWORD";    // PASSWORD FROM SGEM USER TO POST

// Function to initialize the equipment IDs inline, exemple with ID's
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

// Definição da URL do backend
#ifdef USE_EXTERNAL_BACKEND
    #define CREDENTIALS_BACKEND_URL "https://actions-fastapi.onrender.com" // Exemple backend URL
#else
    #define CREDENTIALS_BACKEND_URL "http://192.168.2.155:7022" // Backend LOCAL from development environment
#endif

#endif  /* CREDENTIALS_H */
```

### Passo 3: Configuração dos IDs dos Equipamentos

A função `initializeEquipmentIds` é usada para inicializar os IDs dos equipamentos. Este código já está incluído no bloco de código acima.

### Passo 4: Definição da URL do Backend

No código fornecido, a URL do backend é definida condicionalmente com base na definição de `USE_EXTERNAL_BACKEND`.

- Se `USE_EXTERNAL_BACKEND` estiver definido, a URL será `https://actions-fastapi.onrender.com` (backend do site).
- Caso contrário, a URL padrão será `http://192.168.2.155:7022` (localhost).

### Passo 5: Salvando o Arquivo

Após inserir o código, salve o arquivo `Credentials.h` no diretório `src/`.

## Conclusão

Você configurou com sucesso o arquivo `Credentials.h` com as credenciais de Wi-Fi, login, IDs de equipamentos e URL do backend. Este arquivo é essencial para o funcionamento correto do cliente HTTPS no ESP32.

`Este tutorial cobre todos os passos necessários para preencher e configurar o arquivoc 'Credentials.h' corretamente e como gerar o certificado para determinado site ao fazer requisições HTTPS via SSL/TLS.`
