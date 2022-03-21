### task_0
`cp /box/hpl-2.3.tar ~/ `            - копируем архив

`tar -xvf hpl-2.3.tar`               - разархивируем

`mv hpl-2.3 hpl   `                  - переименовываем

`cp setup/Make.Lnux_PII_CBLAS .`      - создаем файлик на основе существующего

`make arch=Linux_PII_CBLAS`

`pkg-config --flags-only-I openblas` - находим путь до openblas

Получили: `/gnu/store/bs9pl1f805ins80xaf4s3n35a0x2lyq3-openblas-0.3.9`

`vim /gnu/store/bs9pl1f805ins80xaf4s3n35a0x2lyq3-openblas-0.3.9/lib/`

Получили: `lib/pkgconfig/openblas.pc`

`whereis mpicc`                      - находим путь до mpicc

Получили: `/gnu/store/ha85ypr90kskmr9s8xl7j6bpw6ymlb68-profile/bin/mpicc`

В `Linux_PII_CBLAS` меняем:
* `MPdir        = # /usr/local/mpi`
* `MPinc        = # -I$(MPdir)/include`
* `MPlib        = # $(MPdir)/lib/libmpich.a`
* `LAdir        = /gnu/store/bs9pl1f805ins80xaf4s3n35a0x2lyq3-openblas-0.3.9`
* `LAinc        = $(LAdir)/include`
* `LAlib        = $(LAdir)/lib/pkgconfig/openblas.pc`
* `CC = /gnu/store/ha85ypr90kskmr9s8xl7j6bpw6ymlb68-profile/bin/mpicc`

Собираем всё: `make arch=Linux_PII_CBLAS`
