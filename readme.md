# Servidor HTTP Multithreaded

## Programación en Redes - Universidad de Palermo
**Profesor:** Diego Marafetti

## Descripción del Proyecto

Este proyecto implementa un servidor HTTP básico en C con tres modos diferentes de manejo de conexiones concurrentes. El servidor es capaz de servir contenido HTML, responder a peticiones ping y servir archivos de imagen JPEG.

## Características Principales

- **Servidor HTTP** con soporte para peticiones GET
- **3 modos de operación** para manejo de concurrencia
- **Cliente HTTP** de prueba incluido
- **Modo debug** para visualizar las peticiones HTTP
- Manejo de archivos estáticos (imágenes JPEG)
- Respuesta a endpoints específicos (`/`, `/ping`)

## Modos de Operación

### Modo 0: Thread por Conexión
- Crea un nuevo thread para cada conexión entrante

### Modo 1: Select (Single-threaded)
- Utiliza `select()` para multiplexar conexiones

### Modo 2: Thread Pool
- Utiliza la librería `threadpool` proporcionada por el profesor

## Estructura del Proyecto

```
.
├── main.c          # Punto de entrada del servidor
├── server.c        # Implementación de los modos de servidor
├── server.h        # Headers del servidor
├── handler.c       # Manejo de peticiones HTTP
├── handler.h       # Headers del handler
├── config.h        # Configuración global (debug)
├── client.c        # Cliente HTTP de prueba
├── threadpool.c    # Librería thread pool (proporcionada)
├── threadpool.h    # Headers thread pool
└── Makefile        # Compilación del proyecto
```

## Compilación

El proyecto incluye un Makefile para facilitar la compilación:

```bash
# Compilar todo
make

# Compilar solo el servidor
make server

# Compilar solo el cliente
make client

# Limpiar archivos compilados
make clean
```

## Uso

### Servidor

```bash
./server <modo> <puerto> [-d]
```

**Parámetros:**
- `modo`: 0 (thread por conexión), 1 (select), 2 (thread pool)
- `puerto`: Puerto TCP donde escuchar (ej: 8080)
- `-d`: (Opcional) Activa el modo debug

**Ejemplos:**
```bash
# Servidor en modo thread por conexión, puerto 8080
./server 0 8080

# Servidor en modo select, puerto 3000, con debug
./server 1 3000 -d

# Servidor en modo thread pool, puerto 8080
./server 2 8080
```

### Cliente

```bash
./client [puerto]
```

**Parámetros:**
- `puerto`: (Opcional) Puerto del servidor. Por defecto: 8080

**Ejemplo:**
```bash
# Conectar al servidor en puerto por defecto
./client

# Conectar al servidor en puerto 3000
./client 3000
```

## Endpoints Disponibles

| Endpoint | Descripción | Respuesta |
|----------|-------------|-----------|
| `/` | Página principal | HTML con mensaje de bienvenida |
| `/ping` o `/Ping` | Verificación de estado | Texto plano: "pong" |
| `/<nombre>` | Servir imagen | Busca `<nombre>.jpg` y la sirve |

## Funcionamiento del Servidor

### Manejo de Peticiones HTTP

1. El servidor recibe la petición HTTP
2. Parsea el verbo HTTP, ruta y versión
3. Según la ruta solicitada:
   - `/`: Devuelve HTML básico
   - `/ping`: Devuelve "pong" como texto plano
   - Otra ruta: Intenta servir un archivo `.jpg` con ese nombre
4. Si no encuentra el archivo, devuelve error 404

## Detalles de Implementación

### Modo Debug

Cuando se activa con `-d`, el servidor imprime:
- Verbo HTTP de la petición
- Ruta solicitada
- Versión del protocolo HTTP