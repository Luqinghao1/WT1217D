#ifndef FITTINGWIDGET_H
#define FITTINGWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QFutureWatcher>
#include <QTableWidget>
#include <QJsonObject>
#include "modelmanager.h"
#include "mousezoom.h"
#include "chartsetting1.h"

// 数据加载对话框 (保持原有逻辑不变)
class QComboBox;
class FittingDataLoadDialog : public QDialog {
    Q_OBJECT
public:
    explicit FittingDataLoadDialog(const QList<QStringList>& previewData, QWidget *parent = nullptr);
    int getTimeColumnIndex() const;
    int getPressureColumnIndex() const;
    int getDerivativeColumnIndex() const;
    int getSkipRows() const;
    int getPressureDataType() const;
private:
    QTableWidget* m_previewTable;
    QComboBox *m_comboTime, *m_comboPressure, *m_comboDeriv, *m_comboSkipRows, *m_comboPressureType;
    void validateSelection();
};

namespace Ui { class FittingWidget; }

struct FitParameter {
    QString name;
    QString displayName;
    QString symbol;
    QString unit;
    double value;
    bool isFit;
    double min;
    double max;
};

class FittingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FittingWidget(QWidget *parent = nullptr);
    ~FittingWidget();

    void setModelManager(ModelManager* m);
    // 设置观测数据
    void setObservedData(const QVector<double>& t, const QVector<double>& p, const QVector<double>& d);

    // 更新基础参数默认值
    void updateBasicParameters();

    // [新增] 从 JSON 数据加载拟合状态
    void loadFittingState(const QJsonObject& data = QJsonObject());

    // [新增] 获取当前拟合状态的 JSON 对象（用于保存）
    QJsonObject getJsonState() const;

signals:
    void fittingCompleted(ModelManager::ModelType modelType, const QMap<QString, double>& parameters);
    void sigIterationUpdated(double error, QMap<QString, double> currentParams, QVector<double> t, QVector<double> p, QVector<double> d);
    void sigProgress(int progress);

    // [新增] 请求保存信号，发送给父级 FittingPage 处理
    void sigRequestSave();

private slots:
    void on_btnLoadData_clicked();
    void on_btnRunFit_clicked();
    void on_btnStop_clicked();
    void on_btnImportModel_clicked();
    void on_btnExportData_clicked();
    void on_btnExportChart_clicked();
    void on_btnResetParams_clicked();
    void on_btnResetView_clicked();
    void on_btnChartSettings_clicked();
    void on_btn_modelSelect_clicked();

    // [修改] 点击保存按钮，只触发信号
    void on_btnSaveFit_clicked();
    // [修改] 导出报告，支持中英文字体
    void on_btnExportReport_clicked();

    void onIterationUpdate(double err, const QMap<QString,double>& p, const QVector<double>& t, const QVector<double>& p_curve, const QVector<double>& d_curve);
    void onFitFinished();

private:
    Ui::FittingWidget *ui;
    ModelManager* m_modelManager;
    MouseZoom* m_plot;
    QCPTextElement* m_plotTitle;
    ModelManager::ModelType m_currentModelType;

    QList<FitParameter> m_parameters;

    QVector<double> m_obsTime;
    QVector<double> m_obsPressure;
    QVector<double> m_obsDerivative;

    bool m_isFitting;
    bool m_stopRequested;
    QFutureWatcher<void> m_watcher;

    void setupPlot();
    void initializeDefaultModel();
    void loadParamsToTable();
    void updateParamsFromTable();
    void updateModelCurve();

    void runOptimizationTask(ModelManager::ModelType modelType, QList<FitParameter> fitParams, double weight);
    void runLevenbergMarquardtOptimization(ModelManager::ModelType modelType, QList<FitParameter> params, double weight);

    QVector<double> calculateResiduals(const QMap<QString, double>& params, ModelManager::ModelType modelType, double weight);
    QVector<QVector<double>> computeJacobian(const QMap<QString, double>& params, const QVector<double>& residuals, const QVector<int>& fitIndices, ModelManager::ModelType modelType, const QList<FitParameter>& currentFitParams, double weight);
    QVector<double> solveLinearSystem(const QVector<QVector<double>>& A, const QVector<double>& b);
    double calculateSumSquaredError(const QVector<double>& residuals);

    QStringList parseLine(const QString& line);
    void getParamDisplayInfo(const QString& key, QString& outName, QString& outSymbol, QString& outUnicodeSymbol, QString& outUnit);
    QStringList getParamOrder(ModelManager::ModelType type);
    void plotCurves(const QVector<double>& t, const QVector<double>& p, const QVector<double>& d, bool isModel);

    // 辅助：获取图片 Base64
    QString getPlotImageBase64();
};

#endif // FITTINGWIDGET_H
