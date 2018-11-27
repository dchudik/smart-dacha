# Smart-Dacha
## Автор: Chudinov Dima (dchudik)
#### Специально для Конкурса .....
   1. Для начало установите OpenHab v2 на RaspberryPI
       
   2. Установите конфигурацию для OpenHab:
       Скопируйте файлы из папок Rules, Sitemap, Items, Things в соответствующие папки.
   3. Скопируйте скрипт связки mqtt и http на RaspberryPi вместе с виртуальным окружением (venv)
   4. Активируйте виртуальное окружение `$source venv/bin/activate`
   5. Запустите скрипт c помощью команды: `$python script.py`
   6. Перейдите по адресу http://ip-raspberry-pi:8080
   7. Во вкладке Basic UI, будет доступен sitemap - dacha, перейдя в него, у вас откроется интерфейс "Умной Дачи"
