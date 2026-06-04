from django.contrib import admin
from django.urls import path

from mainapp.views import CamerasViewSet
from health_check.views import HealthCheckView

urlpatterns = [
    path('admin/', admin.site.urls),
    # path('health/', HealthCheckView.as_view(checks=['health_check.checks.Database', 'health_check.checks.Storage'])),
    
    path('health/', HealthCheckView.as_view(checks=[
        'health_check.checks.Database',  # подключение к БД
        'health_check.checks.Cache', # кэш
        'health_check.checks.Storage', # файловое хранилище
        'health_check.checks.DNS', # DNS-резолвинг
        # "health_check.Mail", # почта
    ]), name='health-check'),

    
    path('api/cameras', CamerasViewSet.as_view({'get': 'list', 'post': 'create'})),
    path('api/cameras/<pk>', CamerasViewSet.as_view({'get': 'retrieve', 'put': 'update', 'patch': 'partial_update', 'delete': 'destroy'})),
]
