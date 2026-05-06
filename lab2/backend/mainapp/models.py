from django.db import models

# Create your models here.

class Cameras(models.Model):
    brand = models.CharField(max_length=50)
    release_year = models.IntegerField()
    megapixels = models.DecimalField(max_digits=5, decimal_places=1)
    sensor_type = models.CharField(max_length=20)
    interchangeable_lens = models.BooleanField()
    price = models.IntegerField()