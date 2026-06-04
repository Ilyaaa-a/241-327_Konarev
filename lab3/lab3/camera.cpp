#include "camera.h"

Camera Camera::fromJson(const QJsonObject &obj)
{
    Camera c;
    c.id = obj.value("id").toInt();
    c.brand = obj.value("brand").toString();
    c.release_year = obj.value("release_year").toInt();
    c.megapixels = obj.value("megapixels").toString();
    c.sensor_type = obj.value("sensor_type").toString();
    c.interchangeable_lens = obj.value("interchangeable_lens").toBool();
    c.price = obj.value("price").toInt();
    return c;
}

QJsonObject Camera::toJson(bool includeId) const
{
    QJsonObject obj;
    if (includeId) {
        obj["id"] = id;
    }
    obj["brand"] = brand;
    obj["release_year"] = release_year;
    obj["megapixels"] = megapixels;
    obj["sensor_type"] = sensor_type;
    obj["interchangeable_lens"] = interchangeable_lens;
    obj["price"] = price;
    return obj;
}
