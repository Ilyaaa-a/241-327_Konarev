# mainapp/gentestdata.py
import random
from datetime import date
from faker import Faker
from django.db import transaction
from .models import Cameras

fk = Faker('ru_RU')

def gentestdata():
    from django.db import connection

    # Удаление старых записей
    Cameras.objects.all().delete()
    cursor = connection.cursor()
    cursor.execute("ALTER SEQUENCE mainapp_cameras_id_seq RESTART WITH 1;")
    print('удалены все записи, сброшен автоинкремент')
    
    with transaction.atomic():
        for _ in range(100):
            Cameras.objects.create(
                brand=random.choice(['Canon', 'Nikon', 'Sony', 'Fujifilm', 'Olympus', 'Panasonic']),
                release_year=random.randint(2005, 2026),
                megapixels=round(random.uniform(12.0, 60.0), 2),
                sensor_type=random.choice(['Full_Frame', 'APS_C', 'Micro_4/3']),
                interchangeable_lens=random.choice([True, False]),
                price=round(random.uniform(10000, 500000), 2),
            )
    print('создано 100 записей')


# python manage.py shell
# from mainapp.gentestdata import gentestdata
# gentestdata()