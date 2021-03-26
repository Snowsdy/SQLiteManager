#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>
#include "user.h"

class Admin : public User
{
    Q_OBJECT
public:
    Admin();
    int equals(Admin a);
};

#endif // ADMIN_H
