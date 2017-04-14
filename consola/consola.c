
#include "consola.h"


void cargarConfiguracion(){


	char* pat = string_new();
		char cwd[1024]; // Variable donde voy a guardar el path absoluto hasta el /Debug
		string_append(&pat,getcwd(cwd,sizeof(cwd)));
		string_append(&pat,"/consola.cfg");
		t_config* configConsola = config_create(pat);
		printf("El directorio sobre el que se esta trabajando es %s\n", pat);
		free(pat);
		if (config_has_property(configConsola, "IP_KERNEL"))
				config.IP_KERNEL = config_get_string_value(configConsola,"IP_KERNEL");
		if (config_has_property(configConsola, "PUERTO_KERNEL"))
				config.PUERTO_KERNEL = config_get_int_value(configConsola,"PUERTO_KERNEL");
}

int crearSocket(){
	return socket(AF_INET,SOCK_STREAM,0);
}

int conectarSocket(int socket, struct sockaddr_in* direccionServidor){
	return connect(socket, (struct sockaddr*) &*direccionServidor, sizeof(struct sockaddr));
}

void cerrarSocket(int socket){
	close(socket);
}

int main (void){

    cargarConfiguracion();

    int cliente = crearSocket();
	if(cliente == -1){  // Se valida de que se pudo crear el socket sin inconvenientes, retornando de "crearSocket un valor >=0.En caso de devolver -1 se visualizará el error.
		perror("No se pudo crear el socket correctamente");
	}

	struct sockaddr_in direccionServidor; //Creo y configuro el servidor
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(config.IP_KERNEL);
	direccionServidor.sin_port = htons(config.PUERTO_KERNEL);

	if(conectarSocket(cliente, &direccionServidor) != 0){ // no se está conectando al servidor
		perror("No se realizó la conexión");
	}

	while (1) {
	char mensaje[1000];
	scanf("%s", mensaje);
	send(cliente, mensaje, strlen(mensaje), 0);
	}


	return 0;
}