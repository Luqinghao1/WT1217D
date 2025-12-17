#ifndef MODELWIDGET6_H
#define MODELWIDGET6_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <functional>
#include <tuple>
#include <QLineEdit>

#include "mousezoom.h"
#include "chartsetting1.h"

// 定义模型曲线数据类型: 时间, 压力, 压力导数
typedef std::tuple<QVector<double>, QVector<double>, QVector<double>> ModelCurveData;

namespace Ui {
class ModelWidget6;
}

class ModelWidget6 : public QWidget
{
    Q_OBJECT

public:
    explicit ModelWidget6(QWidget *parent = nullptr);
    ~ModelWidget6();

    // 计算理论曲线
    ModelCurveData calculateTheoreticalCurve(const QMap<QString, double>& params,
                                             const QVector<double>& providedTime = QVector<double>());

    void setHighPrecision(bool high);

private slots:
    void onCalculateClicked();
    void onResetParameters();
    void onExportData();
    void onExportImage();
    void onResetView();
    void onFitToData();
    void onChartSettings();
    void onDependentParamsChanged();
    void onShowPointsToggled(bool checked);

signals:
    void calculationCompleted(const QString &analysisType, const QMap<QString, double> &results);

private:
    void initChart();
    void setupConnections();

    QVector<double> parseInput(const QString& text);
    void setInputText(QLineEdit* edit, double value);
    void runCalculation();
    void plotCurve(const ModelCurveData& data, const QString& name, QColor color, bool isSensitivity);

    void calculatePDandDeriv(const QVector<double>& tD, const QMap<QString, double>& params,
                             std::function<double(double, const QMap<QString, double>&)> laplaceFunc,
                             QVector<double>& outPD, QVector<double>& outDeriv);

    // 拉普拉斯空间解函数 (复合油藏)
    double flaplace_composite(double z, const QMap<QString, double>& p);

    // 无穷大/有界地层压力解 (包含 reD 参数)
    double PWD_inf(double z, double fs1, double fs2, double M12, double LfD, double rmD, double reD, int nf, const QVector<double>& xwD);

    // 辅助数学函数
    double scaled_besseli(int v, double x);
    double adaptiveGauss(std::function<double(double)> f, double a, double b, double eps, int depth, int maxDepth);
    double gauss15(std::function<double(double)> f, double a, double b);
    double stefestCoefficient(int i, int N);
    double factorial(int n);

private:
    Ui::ModelWidget6 *ui;
    MouseZoom *m_plot;
    QCPTextElement *m_plotTitle;
    bool m_highPrecision;

    QVector<double> res_tD;
    QVector<double> res_pD;
    QVector<double> res_dpD;

    QList<QColor> m_colorList;
};

#endif // MODELWIDGET6_H
