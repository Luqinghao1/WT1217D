#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QString>
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <tuple>

class ModelWidget1;
class ModelWidget2;
class ModelWidget3;
class ModelWidget4;
class ModelWidget5;
class ModelWidget6;

typedef std::tuple<QVector<double>, QVector<double>, QVector<double>> ModelCurveData;

class ModelManager : public QObject
{
    Q_OBJECT

public:
    enum ModelType {
        Model_1 = 0,
        Model_2,
        Model_3,
        Model_4,
        Model_5,
        Model_6
    };
    Q_ENUM(ModelType)

    explicit ModelManager(QWidget* parent = nullptr);
    ~ModelManager();

    void initializeModels(QWidget* parentWidget);
    void switchToModel(ModelType modelType);

    // 获取当前模型名称
    static QString getModelTypeName(ModelType type);

    // 获取默认参数 (现在会从全局参数读取)
    QMap<QString, double> getDefaultParameters(ModelType type);

    // 计算理论曲线
    ModelCurveData calculateTheoreticalCurve(ModelType type, const QMap<QString, double>& params,
                                             const QVector<double>& providedTime = QVector<double>());

    // 生成对数时间步长
    static QVector<double> generateLogTimeSteps(int count, double startExp, double endExp);

    // 设置/获取观测数据
    void setObservedData(const QVector<double>& t, const QVector<double>& p, const QVector<double>& d);
    void getObservedData(QVector<double>& t, QVector<double>& p, QVector<double>& d) const;
    bool hasObservedData() const;

    // 设置高精度计算模式
    void setHighPrecision(bool high);

    // [新增] 通知所有模型组件更新基础参数
    void updateAllModelsBasicParameters();

signals:
    void calculationCompleted(const QString &analysisType, const QMap<QString, double> &results);
    void modelSwitched(ModelType newModel, ModelType oldModel);

private slots:
    void onSelectModelClicked();
    void onWidgetCalculationCompleted(const QString &analysisType, const QMap<QString, double> &results);

private:
    void createMainWidget();
    void setupModelSelection();
    void connectModelSignals();

    QWidget* m_mainWidget;
    QPushButton* m_btnSelectModel;
    QStackedWidget* m_modelStack;

    ModelWidget1* m_modelWidget1;
    ModelWidget2* m_modelWidget2;
    ModelWidget3* m_modelWidget3;
    ModelWidget4* m_modelWidget4;
    ModelWidget5* m_modelWidget5;
    ModelWidget6* m_modelWidget6;

    ModelType m_currentModelType;

    // 缓存的观测数据
    QVector<double> m_cachedObsTime;
    QVector<double> m_cachedObsPressure;
    QVector<double> m_cachedObsDerivative;
};

#endif // MODELMANAGER_H
