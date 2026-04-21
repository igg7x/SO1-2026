# Nombre del ejecutable
TARGET = ej3

# Compilador y banderas
CC = gcc
# -O2 activa optimizaciones que favorecen el reordenamiento
# -pthread es necesario para linkear la librería de hilos
CFLAGS = -O2 -pthread -Wall

# Regla principal
all: $(TARGET)

$(TARGET): ej3.c
	$(CC) $(CFLAGS) ej3.c -o $(TARGET)

# Regla para probar el programa repetidamente
# Ejecuta el programa hasta que falle (el assert cause un core dump)
test: $(TARGET)
	@echo "Ejecutando bucle de prueba... Presiona Ctrl+C para detener."
	@i=0; while ./$(TARGET); do \
		i=$$((i+1)); \
		if [ $$((i % 1000)) -eq 0 ]; then echo "Intento $$i..."; fi; \
	done

# Limpieza
clean:
	rm -f $(TARGET)