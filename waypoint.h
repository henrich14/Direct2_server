#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QObject>
#include <QDebug>

class Waypoint
{
public:
    Waypoint(double x=0, double y=0, double altitude=0, QString label="WP", bool mandatory=true, QObject *parent = 0);

    const double x() const;
    const double y() const;
    const double altitude() const;
    const QString label() const;
    const bool mandatory() const;

    Waypoint setMandatory(const bool mndtry);

    bool operator==(const Waypoint &WP)
    {
        return (m_x == WP.x()) && (m_y == WP.y()) && (m_altitude == WP.altitude());
    }

    Waypoint& operator=(const Waypoint &WP)
    {
        m_x = WP.m_x;
        m_y = WP.m_y;
        m_altitude = WP.m_altitude;
        m_label = WP.m_label;
        m_mandatory = WP.m_mandatory;

        return *this;
    }

private:
    double m_x;
    double m_y;
    double m_altitude;
    QString m_label;
    bool m_mandatory;
};
#endif // WAYPOINT_H
