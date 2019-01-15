#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QButtonGroup>
#include <QString>
#include <QTime>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGraphicsView>
#include <QTextBrowser>
#include <QSizePolicy>

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include <memory>
#include <fstream>
#include <string>

#include "generateRandom.hpp"
#include "Factors.hpp"
#include "TimeRange.hpp"
#include "Action.hpp"
#include "Sleep.hpp"
#include "Coffee.hpp"
#include "Chromosome.hpp"
#include "DayActions.hpp"
#include "GoalFunction.hpp"
#include "CoffeeAndSleepGoalFunction.hpp"
#include "GeneticAlgorithm.hpp"
#include "Point.hpp"


namespace Ui
    {
    class MainWindow;
    }

class MainWindow : public QMainWindow
    {
    Q_OBJECT

    private:
        Ui::MainWindow *ui;
        std::shared_ptr<Sleep> sleep;
        std::shared_ptr<Coffee> coffee;
        TimeRange range;
        Factors st_factors;
        std::vector<shared_ptr_Action> vec;
        std::vector<checkingFunction> fun;
        std::shared_ptr<CoffeeAndSleepGoalFunction> gF;
        std::shared_ptr<DayActions> dayAction;
		std::vector<std::shared_ptr<DayActions> > vec_DayActions;
		std::vector<stringAndParseFunction> parse_form;

    private:

		//ustawienia startowego DayActions
		int y = 50;
		double a = -0.1 ;
		QTime coffee_moment = QTime(12,15);
		int coffee_quantity = 60;
        QTime sleep_begin = QTime(0,0);
		QTime sleep_end = QTime(8,0);
        QTime del_beg = QTime(0,0);
        QTime del_end = QTime(0,0);

		void updateDayActionDistplay();
		void updateStartPopulation();


        //ustawienia statyczne action dayaction sleep goalfunction itp.
        int min_coffee = 60;
        int max_total_coffee = 1500;
        double coffee_a_multiplier = 1;
        double coffee_y_multiplier = 1;

        int min_sleep = 20;
        int min_total_sleep = 180;
		double sleep_a_multiplier = 3;

        double integral_multiplier = 1.0;
		double coffee_multiplier = 1.0;

		// odczyt i zapis z pliku
		std::string plik_input = "";
		std::string plik_output = "";


		std::string staticParamToString();
		bool fromFileStaticParam(std::istream& in);
		void updateDisplayStaticParam();




    private:
        //ustawienia parametrów algorytmu
		double min_difference = 0.0001;
		unsigned int max_iter = 100;
		unsigned int max_best = 10;
		unsigned int max_population_size = 200;
		unsigned int number_of_repeat = 2;
        unsigned int cycle_length = 0;
		std::string file_to_save = "";
		std::string file_to_load = "";


        //obiekty algorytmu
        std::shared_ptr<GeneticAlgorithm> algorithm = nullptr;
        std::shared_ptr<Chromosome> best = nullptr;

        std::stringstream algorithm_buffor;

        void updateDisplaying();
        void setAlgorithmParameters();
		void addElementToQScroll(Chromosome* ch,QWidget* widget, QVBoxLayout* boxLayout, int num);
		void clearLayout(QLayout* lout);

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        //dodawanie akcji itp.
        void whenStartFactorsY(int);
        void whenStartFactorsA(double);
        void whenUstawClicked();

        void whenDelBegChanged(QTime);
        void whenDelEndChanged(QTime);
        void whenDeleteRangeClicked();

        void whenCoffeeQuantity(int);
        void whenCoffeeMomentChanged(QTime);
        void whenDodajKaweClicked();

        void whenSleepBeginChanged(QTime);
        void whenSleepEndChanged(QTime);
        void whenDodajSenClicked();

        void whenSprawdzOgraniczeniaIPoprawClicked();
        void whenUsunAkcjeClicked();
		void whenDodajDoListyClicked(); ///x
		void whenDodajLosoweClicked();  ///x

        //pola współczynnikóW statycznych

        void whenMinCoffee(int);
        void whenMaxCoffee(int);
        void whenYCoffeeMod(double);
        void whenACoffeeMod(double);
        void whenASleepMod(double);
        void whenMinSleep(int);
        void whenMinTotSleep(int);
        void whenIntegerFactor(double);
        void whenCoffeeFactor(double);
        void whenToAlgorithm();
        void whenSetStaticFactors();

		//odczt i zapis do pliku page 1
		void whenSaveFile(QString);
		void whenLoadFile(QString);

		void whenLoadStartObjectClicked();
		void whenLoadStartPopulationClicked();
		void whenLoadStaticParametersClicked();

		void whenSaveStartObjectClicked();
		void whenSaveStartPopulationClicked();
		void whenSaveStaticParametersClicked();



		//odczyt i  zapis parametrów algorytmu
		void whenLoadAlgParamFile(QString);

		void whenSavePopulationClicked();
		void whenSaveBestClicked();
		void whenSaveAlgorithmParametersClicked();
		void whenSaveStatusClicked();
		void whenLoadAlgSettingsClicked();

		void whenSaveGoalTraceClicked();


        //parametry algorytmu
        void whenMinDiff(double);
        void whenMaxIter(int);
		void whenFileToSave(QString);
        void whenMaxBest(int);
        void whenMaxPopulation(int);
		void whenCycleSize(int);
        void whenNumberOfRepeat(int);



        void whenStartButtonClicked();
        void whenResetButtonClicked();
        void whenContinueButtonClicked();

        void whenChangeStartObjectButton();
        void whenQuitButton();


    signals:
        void updateDisplaingDayActions(QString);
        void showBest(QString);
        void quit();

    };


class View : public QGraphicsView
	{
	private:
		QPainterPath path;  //sciezka wykresu
		QPainterPath origin;  //wartosci i chwile czasu day action
		int plot_width = 1440 + 1;
		int plot_heigh = 100 + 1;

	protected:
		QGraphicsScene* scene = new QGraphicsScene(QRectF(0,0,width(), height()),this);
		QPointF point; //translacja

		virtual void resizeEvent(QResizeEvent *event);

		void reScale();
		virtual QPointF countTranslation();
		virtual double countMultiplierX(int place_for_plot);
		virtual double countMultiplierY(int place_for_plot);


	public:
		View(QWidget* parent = nullptr, int plot_w = 1441,int plot_h  =101);

		virtual void draw(const Chromosome* ch);
		virtual void draw(const std::vector<double>& tr);

		virtual ~View()
			{}

		void setPlotW(int w)
			{
			plot_width = w;
			}

		void setPlotH(int h)
			{
			plot_heigh = h;
			}
		int getPlotWidth()
			{
			return plot_width;
			}

		int getPlotHeigh()
			{
			return plot_heigh;
			}
	};


class ScaleView : public View
	{
	private:
		int scale_x, scale_y;

		bool as_hour = true;

		QString text_x  =tr("Czas[h, min]");
		QString text_y = tr("y[%]");

		QPainterPath axis_path;
		QPainterPath axis_path_origin;
		QPolygonF triangle_up;
		QPolygonF triangle_right;

								  // g, p, d, l
		std::vector<int> padding = {10,50,50,40};

		void addScaleX(int scale_x, int len_x, int font_size, QFont& font);
		void addScaleY(int scale_y, int len_y, int font_size, QFont& font);

	protected:
		void resizeEvent(QResizeEvent *event);
		QPointF countTranslation();

		double countMultiplierX(int place_for_plot);//od place odjąć 2x place_nex_to_plot
		double countMultiplierY(int place_for_plot);
		double axisMultiplierX(int place_for_plot);//od place odjąć 2x place_nex_to_plot
		double axisMultiplierY(int place_for_plot);

		void scaleAxis();
		void drawAxis();


	public:

		ScaleView(QWidget* parent = nullptr, int plot_w = 1441,int plot_h  =101, int sc_x = 5, int sc_y = 4, std::vector<int> padding = {10,50,50,40});

		void setTextX(QString s)
			{
			text_x = s;
			}

		void setTextY(QString s)
			{
			text_y = s;
			}

		void setScaleX(int sc)
			{
			scale_x = sc;
			}

		void setScaleY(int sc)
			{
			scale_y = sc;
			}

		void setAsHour(bool h)

			{
			as_hour = h;
			}

		virtual void draw(const Chromosome* ch);
		virtual void draw(const std::vector<double>& tr);
	};















#endif // MAINWINDOW_HPP
