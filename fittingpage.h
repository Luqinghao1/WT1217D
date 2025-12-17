#ifndef FITTINGPAGE_H
#define FITTINGPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QTabWidget>
#include "modelmanager.h"

// 前置声明
class FittingWidget;

namespace Ui {
class FittingPage;
}

class FittingPage : public QWidget
{
    Q_OBJECT

public:
    explicit FittingPage(QWidget *parent = nullptr);
    ~FittingPage();

    // 设置模型管理器（传递给子页面）
    void setModelManager(ModelManager* m);

    // 接收来自 MainWindow 的数据，传递给当前激活的 FittingWidget
    void setObservedDataToCurrent(const QVector<double>& t, const QVector<double>& p, const QVector<double>& d);

    // 初始化/重置基本参数
    void updateBasicParameters();

    // 从项目文件加载所有拟合分析的状态
    void loadAllFittingStates();

    // 保存所有拟合分析的状态到项目文件
    void saveAllFittingStates();

private slots:
    // 新建分析页签
    void on_btnNewAnalysis_clicked();
    // 重命名当前页签
    void on_btnRenameAnalysis_clicked();
    // 删除当前页签
    void on_btnDeleteAnalysis_clicked();

    // 响应子页面发出的保存请求
    void onChildRequestSave();

private:
    Ui::FittingPage *ui;
    ModelManager* m_modelManager;

    // 创建一个新的拟合页的内部函数
    // name: 页签名称
    // initData: 初始状态数据（如果是复制或加载存档，否则为空）
    FittingWidget* createNewTab(const QString& name, const QJsonObject& initData = QJsonObject());

    // 生成唯一的页签名称（防止重名）
    QString generateUniqueName(const QString& baseName);
};

#endif // FITTINGPAGE_H
