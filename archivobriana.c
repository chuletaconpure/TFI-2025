//encabezado
void cargar_respuestas_csv(struct lista2 *L, const char *RespuestasCSV);

//llamado
cargar_respuestas_csv(listaRespuestas, "respuestas.csv");

//funcion
void cargar_respuestas_csv(struct lista2 *L, const char *RespuestasCSV) {
    struct lista2 *aux = NULL;

    FILE *archivo = fopen(RespuestasCSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    } else {
        fprintf(archivo, "Respuesta_Id;Pregunta_Id;Respuesta_Nro;Respuesta;Ponderacion;Elegida\n");
        aux = L;
        while (aux != NULL) {
            fprintf(archivo, "%d;%d;%d;\"%s\";%.2f;%d\n", //las comillas son para separar las comas del formato excell nomas
  	        aux->Respuesta_Id,
  	        aux->Pregunta_Id,
  	        aux->Respuesta_Nro,
  	        aux->Respuesta,
  	        aux->Ponderacion,
  	        aux->Elegida);
            aux = aux->sgte;
        }
    }

    fclose(archivo);
    printf("Respuestas cargadas en el archivo %s\n", RespuestasCSV);
}
