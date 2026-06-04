#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QJsonObject>

// пример
// {
//   "id": 1, "brand": "Olympus", "release_year": 2016,
//   "megapixels": "19.1", "sensor_type": "Micro_4/3",
//   "interchangeable_lens": false, "price": 24991
// }

class Camera
{
public:
    int id = 0;
    QString brand;
    int release_year = 0;
    QString megapixels;
    QString sensor_type;
    bool interchangeable_lens = false;
    int price = 0;

    // десериализация из JSON-объекта в C++ - объект
    static Camera fromJson(const QJsonObject &obj);

    // сериализация C++ - объекта в JSON
    QJsonObject toJson(bool includeId = false) const;
};

#endif
