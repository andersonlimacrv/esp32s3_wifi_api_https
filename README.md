# HTTPS-ESP-HTTPS-Server-Client

Cliente HTTPS usando o ESP-IDF no mesmo ESP32, de forma paralela.

##  Criando Certificados PEM para o Servidor

Para criar os certificados PEM para o lado do servidor, utilize o seguinte comando OpenSSL:

```bash
openssl req -newkey rsa:2048 -nodes -keyout ServerKey.pem -x509 -days 3650 -out ServerCert.pem -subj "/CN=ESP32S3_01_HTTPS"
```

Substitua ESP32S3_01 HTTPS server pelo nome do seu servidor, se desejar.

Passo 2: Criando Certificados PEM para o Cliente
Utilize o comando a seguir para visualizar os certificados do site ao qual deseja se conectar, como por exemplo, https://actions-fastapi.onrender.com:

```bash
openssl s_client -showcerts -connect actions-fastapi.onrender.com:443
```

Copie os certificados exibidos e cole-os em um único arquivo ClientCert.pem para o cliente.

Passo 3: Configurando o Cliente ESP32S3_01
Agora, você pode configurar o seu cliente ESP32S3_01 para se conectar ao site https://www.actions-fastapi.onrender.com utilizando os certificados PEM gerados.

Com estes passos, você terá um servidor e cliente HTTPS funcionando em paralelo no seu ESP32.
