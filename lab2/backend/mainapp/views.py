"""
URL configuration for lab1 project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/6.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""

from rest_framework import viewsets
from .models import Cameras
from .serializers import CamerasSerializer


class CamerasViewSet(viewsets.ModelViewSet):
    """
    ViewSet для работы с моделью Cameras.
    Предоставляет CRUD + List операции:
    - list: GET /api/cameras/ - список всех камер
    - create: POST /api/cameras/ - создание новой камеры
    - retrieve: GET /api/cameras/{id}/ - получение камеры по ID
    - update: PUT /api/cameras/{id}/ - полное обновление камеры
    - partial_update: PATCH /api/cameras/{id}/ - частичное обновление камеры
    - destroy: DELETE /api/cameras/{id}/ - удаление камеры
    """
    queryset = Cameras.objects.all()
    serializer_class = CamerasSerializer
