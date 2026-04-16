from django.contrib import admin
from django.urls import path

from mainapp.views import CamerasViewSet

urlpatterns = [
    path('admin/', admin.site.urls),
    path('cameras', CamerasViewSet.as_view({'get': 'list', 'post': 'create'})),
    path('cameras/<pk>', CamerasViewSet.as_view({'get': 'retrieve', 'put': 'update', 'patch': 'partial_update', 'delete': 'destroy'})),
]
