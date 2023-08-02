#ifndef CXXTP_CLASSCARD_H
#define CXXTP_CLASSCARD_H

#include <QWidget>
#include "SSetting.h"

namespace CXXTP {
QT_BEGIN_NAMESPACE
namespace Ui {
class ClassCard;
}
QT_END_NAMESPACE

/*
 * class ClassCard: Single card display module.
 */
class ClassCard : public QWidget {
  Q_OBJECT

public:
  ClassCard(const Lesson&, int id, QWidget *parent = nullptr);
  ClassCard();
  ~ClassCard() override;
  Ui::ClassCard *ui{};

private:
  const int id;
  QWidget* p;
private slots:
  void elect();
};
} // namespace CXXTP

#endif // CXXTP_CLASSCARD_H
