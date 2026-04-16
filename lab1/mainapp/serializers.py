from rest_framework import serializers
from .models import Cameras


class CamerasSerializer(serializers.ModelSerializer):
    """Сериализатор для модели Cameras."""

    class Meta:
        model = Cameras
        fields = '__all__'
