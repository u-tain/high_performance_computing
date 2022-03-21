### task_0
`cp /box/hpl-2.3.tar ~/ `            - копируем архив

`tar -xvf hpl-2.3.tar`               - разархивируем

`mv hpl-2.3 hpl   `                  - переименовываем

`cp setup/Make.linux_PII_CBLAS`      - создаем файлик на основе существующего

`pkg-config -cflags-only-I openblas` - находим путь до openblas

`whereis mpicc`                      - находим путь до mpicc
