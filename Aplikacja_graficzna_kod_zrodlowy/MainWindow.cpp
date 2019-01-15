#include "ui_MainWindow.h"
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
    using namespace std;
    st_factors = Factors(0,0);
    sleep = make_shared<Sleep>();
    coffee = make_shared<Coffee>();

    //wrzucamy do wektora obiekty wzorcowe
    vec.push_back(shared_ptr_Action(sleep));
    vec.push_back(shared_ptr_Action(coffee));

    //wrzucamy do vectora funkcje
    fun.push_back(Sleep::checkRestrictionAndRetake);
    fun.push_back(Coffee::checkRestrictionAndRetake);

    gF = make_shared<CoffeeAndSleepGoalFunction>();

    dayAction = make_shared<DayActions>(gF,vec,fun,st_factors);

	//vektor do parsowania plików
	stringAndParseFunction p1("Coffee:", Coffee::createFromStream), p2("Sleep:", Sleep::createFromStream);

	parse_form.push_back(p1);
	parse_form.push_back(p2);


    ui->setupUi(this);
    ui->textBrowser->setText(QString::fromStdString(dayAction->toString()));
	whenSetStaticFactors();

	ui->trace->setPlotH(101);
	ui->trace->setPlotW(1);
	ui->trace->setAsHour(false);
	ui->trace->setScaleX(7);
	ui->trace->setTextX("Numer iteracji");

    //współczynniki startowe
    connect(ui->startFactors_y,SIGNAL(valueChanged(int)),this,SLOT(whenStartFactorsY(int)));
    connect(ui->startFactors_a,SIGNAL(valueChanged(double)),this,SLOT(whenStartFactorsA(double)));
    connect(ui->setStartFactors,SIGNAL(clicked()),this,SLOT(whenUstawClicked()));

    //dodawanie snu
    connect(ui->sleepBegin,SIGNAL(userTimeChanged(QTime)),this,SLOT(whenSleepBeginChanged(QTime)));
    connect(ui->sleepEnd,SIGNAL(userTimeChanged(QTime)),this,SLOT(whenSleepEndChanged(QTime)));
    connect(ui->addSleep,SIGNAL(clicked()),this,SLOT(whenDodajSenClicked()));

    //dodawanie kawy
    connect(ui->coffee_quantity,SIGNAL(valueChanged(int)),this,SLOT(whenCoffeeQuantity(int)));
    connect(ui->coffee_moment,SIGNAL(userTimeChanged(QTime)),this,SLOT(whenCoffeeMomentChanged(QTime)));
    connect(ui->addCoffee,SIGNAL(clicked()),this,SLOT(whenDodajKaweClicked()));

	//dodanie losowej czynosci
	connect(ui->addRandom,SIGNAL(clicked()),this,SLOT(whenDodajLosoweClicked()));


	//usuwanie wszystkich czynności z dayAction, spr. ogr.i dodawanie do vectora
    connect(ui->removeAddedActions,SIGNAL(clicked()),this,SLOT(whenUsunAkcjeClicked()));
	connect(ui->check,SIGNAL(clicked()),this,SLOT(whenSprawdzOgraniczeniaIPoprawClicked()));
	connect(ui->addToList,SIGNAL(clicked()),this,SLOT(whenDodajDoListyClicked()));

    //usuwanie przedziału
    connect(ui->deleteRangeBegin,SIGNAL(userTimeChanged(QTime)),this,SLOT(whenDelBegChanged(QTime)));
    connect(ui->deleteRangeEnd,SIGNAL(userTimeChanged(QTime)),this,SLOT(whenDelEndChanged(QTime)));
    connect(ui->deleteRange,SIGNAL(clicked()),this,SLOT(whenDeleteRangeClicked()));

    //współczynniki statyczne
    connect(ui->min_coffee,SIGNAL(valueChanged(int)),this,SLOT(whenMinCoffee(int)));
    connect(ui->max_coffee,SIGNAL(valueChanged(int)),this,SLOT(whenMaxCoffee(int)));
    connect(ui->y_coffee_mod,SIGNAL(valueChanged(double)),this,SLOT(whenYCoffeeMod(double)));
    connect(ui->a_coffee_mod, SIGNAL(valueChanged(double)),this,SLOT( whenACoffeeMod(double)));

    connect(ui->a_sleep_mod, SIGNAL(valueChanged(double)),this,SLOT( whenASleepMod(double)));
    connect(ui->min_sleep, SIGNAL(valueChanged(int)),this,SLOT( whenMinSleep(int)));
    connect(ui->min_tot_sleep, SIGNAL(valueChanged(int)),this,SLOT( whenMinTotSleep(int)));

    connect(ui->integer_factor, SIGNAL(valueChanged(double)),this,SLOT( whenIntegerFactor(double)));
    connect(ui->coffee_factor, SIGNAL(valueChanged(double)),this,SLOT( whenCoffeeFactor(double)));

    connect(ui->set_static_factors, SIGNAL(clicked()),this,SLOT( whenSetStaticFactors()));

	//zapis i odczyt
	connect(ui->saveFile, SIGNAL(textChanged(QString)),this,SLOT( whenSaveFile(QString)));
	connect(ui->loadFile, SIGNAL(textChanged(QString)),this,SLOT( whenLoadFile(QString)));

	connect(ui->saveStartObject, SIGNAL(clicked()),this,SLOT( whenSaveStartObjectClicked()));
	connect(ui->saveStartPopulation, SIGNAL(clicked()),this,SLOT( whenSaveStartPopulationClicked()));
	connect(ui->saveStaticParameters, SIGNAL(clicked()),this,SLOT( whenSaveStaticParametersClicked()));

	connect(ui->loadStartObject, SIGNAL(clicked()),this,SLOT( whenLoadStartObjectClicked()));
	connect(ui->loadStartPopulation, SIGNAL(clicked()),this,SLOT( whenLoadStartPopulationClicked()));
	connect(ui->loadStaticParameters, SIGNAL(clicked()),this,SLOT( whenLoadStaticParametersClicked()));



    //wyjscie i do algorytmu
    connect(ui->toAlgorithm, SIGNAL(clicked()),this,SLOT( whenToAlgorithm()));
    connect(ui->quitVievport1, SIGNAL(clicked()),this,SLOT( whenQuitButton()));

    //parametry algorytmu
    connect(ui->minDiff ,SIGNAL( valueChanged(double) ), this, SLOT( whenMinDiff(double) ) );
    connect(ui->maxIter ,SIGNAL( valueChanged(int) ), this, SLOT( whenMaxIter(int) ) );
	connect(ui->maxBest ,SIGNAL( valueChanged(int) ), this, SLOT( whenMaxBest(int) ) );
    connect(ui->maxPopulation ,SIGNAL( valueChanged(int) ), this, SLOT( whenMaxPopulation(int) ) );
    connect(ui->cycleSize ,SIGNAL( valueChanged(int) ), this, SLOT( whenCycleSize(int) ) );
    connect(ui->numberOfRepeat ,SIGNAL( valueChanged(int) ), this, SLOT( whenNumberOfRepeat(int) ) );

	//przyciski save i load
	connect(ui->file ,SIGNAL( textChanged(QString) ), this, SLOT( whenFileToSave(QString) ) );
	connect(ui->loadParamAlgFile ,SIGNAL( textChanged(QString) ), this, SLOT( whenLoadAlgParamFile(QString) ) );

	connect(ui->savePopulation , SIGNAL( clicked() ), this, SLOT( whenSavePopulationClicked() ));
    connect(ui->saveBest , SIGNAL( clicked() ), this, SLOT( whenSaveBestClicked() ));
    connect(ui->saveAlgorithmParameters , SIGNAL( clicked() ), this, SLOT( whenSaveAlgorithmParametersClicked() ));
	connect(ui->saveStatus , SIGNAL( clicked() ), this, SLOT( whenSaveStatusClicked() ));
	connect(ui->saveGoalTrace , SIGNAL( clicked() ), this, SLOT( whenSaveGoalTraceClicked() ));

	connect(ui->loadAlgSettings , SIGNAL( clicked() ), this, SLOT( whenLoadAlgSettingsClicked() ));


    //Control. Control. You must learn control...
    connect(ui->startButton , SIGNAL( clicked() ), this, SLOT( whenStartButtonClicked() ));
    connect(ui->resetButton , SIGNAL( clicked() ), this, SLOT( whenResetButtonClicked() ));
    connect(ui->continueButton , SIGNAL( clicked() ), this, SLOT( whenContinueButtonClicked() ));

    //wyjscie i wstecz
    connect(ui->changeStartObjectButton , SIGNAL( clicked() ), this, SLOT( whenChangeStartObjectButton() ));
    connect(ui->quitButton , SIGNAL( clicked() ), this, SLOT( whenQuitButton() ));

    //aktualizacja wyswietlania startowego dayAction
    connect(this,SIGNAL(updateDisplaingDayActions(QString)),ui->textBrowser,SLOT(setText(QString)));

    }

MainWindow::~MainWindow()
    {
    delete ui;
    }

//private methods
void MainWindow::updateDayActionDistplay()
    {
	ui->plotDayAction->draw(dayAction.get());
    ui->textBrowser->setText(QString::fromStdString(dayAction->toString()));
    }

void MainWindow::updateStartPopulation()
	{
	//czyszczenie layoutu
	clearLayout(ui->verticalLayout_19);


	int i =0;
	for(std::shared_ptr<DayActions> x : vec_DayActions)
		{
		x->updateFactors();
		addElementToQScroll(x.get(),ui->layout_populacji_startowej,ui->verticalLayout_19,++i);
		}

	}


void MainWindow::updateDisplaying()
    {
    if(best != nullptr)
		{
		ui->plotTheBest->draw(best.get());
		std::stringstream str;
		algorithm->status(str);

		ui->stringTheBest->setText( QString::fromStdString( str.str()) );
		}
    //czyszczenie layoutu
    clearLayout(ui->verticalLayout_15);

    //vector populacji
    const std::vector<Chromosome*>* vec = algorithm->getPopulation();

    int i =0;
    for(Chromosome* x : *vec)
        {
		addElementToQScroll(x,ui->layout_listy,ui->verticalLayout_15,++i);
        }

    }


void MainWindow::setAlgorithmParameters()
    {
    algorithm->setMinDifferenceBetweenGenarationsBest(min_difference);
    algorithm->setMaxOfIteration(max_iter);
    algorithm->setMaxBest(max_best);
    algorithm->setMaxPopulationSize(max_population_size);
    algorithm->setNumberOfRepeat(number_of_repeat);
    }

void MainWindow::addElementToQScroll(Chromosome* ch,QWidget* widget, QVBoxLayout* boxLayout, int num)//=lista_chromosomow
	{

    if(ch == nullptr || boxLayout == nullptr)
        return;

    //QVBoxLayout* verdicalLayout = new QVBoxLayout(grupa);

    //QGraphicsView* plot = new QGraphicsView(grupa);

    //QTextBrowser* opis = new QTextBrowser(grupa);
    QSizePolicy size1(QSizePolicy::Preferred, QSizePolicy::Minimum);//10
    QSizePolicy size2(QSizePolicy::Expanding, QSizePolicy::Fixed);//7


    QGroupBox* grupa = new QGroupBox(widget);
    grupa->setObjectName(QString::fromUtf8("grupa"));
    size1.setHeightForWidth(grupa->sizePolicy().hasHeightForWidth());
    grupa->setSizePolicy(size1);
    grupa->setTitle(QString::fromStdString( "Chromosom nr " + std::to_string(num)+" :"));

	QVBoxLayout* vLayout = new QVBoxLayout(grupa);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(11, 11, 11, 11);
    vLayout->setObjectName(QString::fromUtf8("verticalLayout_17"));  

	ScaleView* plot = new ScaleView(grupa);
    plot->setObjectName(QString::fromUtf8("plot"));
	size2.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
	plot->setSizePolicy(size2);
	plot->setMinimumSize(QSize(grupa->size().width(), 250));

    vLayout->addWidget(plot);

    QTextBrowser* opis = new QTextBrowser(grupa);
    opis->setObjectName(QString::fromUtf8("opis"));
    opis->setMinimumSize(QSize(0, 300));

    vLayout->addWidget(opis);
    boxLayout->addWidget(grupa);

    opis->setText(QString::fromStdString( ch->toString() ));
	plot->draw(ch);
	}


/** View  **/

QPointF View::countTranslation()
	{
	QPointF pt  = QPointF(51-width()/2,13+height()/2);

	return pt;
	}

void View::draw(const Chromosome* ch)
	{
	const DayActions* da = dynamic_cast<const DayActions*>(ch);

	origin = QPainterPath(QPointF(0,0));

//pętla uzupełniania ścieżki
	std::vector<Point> vec = da->getActivityPoints();

	for(Point& x : vec)
		origin.lineTo(QPointF(x.getX(),x.getY()));

	reScale();
	}

void View::resizeEvent(QResizeEvent *event)
	{
	reScale();

	QGraphicsView::resizeEvent(event);
	}

double View::countMultiplierX(int place_for_plot)
	{
	return (place_for_plot - 2)/double(plot_width);
	}

double View::countMultiplierY(int place_for_plot)
	{
	return (place_for_plot - 2)/double(plot_heigh);
	}



void View::reScale()
	{
	int size = origin.elementCount();
	path = origin;

	scene->clear();
	point = countTranslation();

	for(int i = 0; i < size; ++i)
		{
		QPainterPath::Element el = origin.elementAt(i);

		el.x*=countMultiplierX(width());
		el.y= -el.y*countMultiplierY(height());

		path.setElementPositionAt(i,el.x,el.y);
		}

	path.translate(point);
	scene->addPath(path);
	}


void View::draw(const std::vector<double>& tr)
	{
	setPlotW(tr.size());

	origin = QPainterPath(QPointF(0,0));

	//pętla uzupełniania ścieżki
	std::vector<Point> vec;

	for(unsigned int i = 0; i < tr.size(); ++i)
		vec.push_back(Point(i,tr[i]));

	for(Point& x : vec)
		origin.lineTo(QPointF(x.getX(),x.getY()));

	reScale();
	}


View::View(QWidget* parent, int plot_w,int plot_h) : QGraphicsView(parent), plot_width(plot_w), plot_heigh(plot_h)
	{
	this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	point = QPointF(51-width()/2,13+height()/2);

	origin = QPainterPath(QPointF(0,0));

	path = origin;

	reScale();

	//path.moveTo(QPointF(0,-width()/2+1));
	setScene(scene);
	}

/** View  **/


/** ScaleView **/
ScaleView::ScaleView(QWidget* parent, int plot_w,int plot_h, int sc_x, int sc_y, std::vector<int> padding ) : View (parent,plot_w,plot_h)
	{
	this->padding  =padding;
	scale_x = sc_x;
	scale_y = sc_y;

	drawAxis();
	}

QPointF ScaleView::countTranslation()
	{
	QPointF pt = QPointF(51-width()/2 + padding[2], 13+height()/2 - padding[3]);

	return pt;
	}

double ScaleView::countMultiplierX(int place_for_plot)
	{
	return View::countMultiplierX(place_for_plot - padding[1] - padding[3]);
	}

double ScaleView::countMultiplierY(int place_for_plot)
	{
	return View::countMultiplierY(place_for_plot - padding[0] - padding[2]);
	}

void ScaleView::drawAxis()
	{
	setScaleX(width()/200+1);

	const int arrow_w = 2;
	const int arrow_h = 9;
	const int end =  std::min(padding[0],padding[1]);

	const int len_x = width() - padding[1] - padding[3] ;
	const int len_y = -(height() - padding[0] - padding[2]);

	QPointF p0(0,0), p1(len_x + end,0), p2(0,len_y - end);
	QPoint ps1(len_x + end - arrow_h, -arrow_w), ps2(len_x + end - arrow_h, arrow_w),
			ps3(-arrow_w, len_y - end + arrow_h), ps4(arrow_w, len_y - end + arrow_h),

			ps11(len_x + end - arrow_h, -arrow_w+1),ps21(len_x + end - arrow_h, arrow_w-1),//wypełniacze
			ps31(-arrow_w+1, len_y - end + arrow_h),ps41(arrow_w-1, len_y - end + arrow_h);

	//STRZAŁKI
	QVector<QPointF> points = {ps3,p2,ps4,ps31,p2,ps41,ps3};//stralka w gore
	triangle_up = QPolygonF(points);

	QVector<QPointF> points_right = {ps1,p1,ps2,ps11,p1,ps21,ps1};//stralka w prawo
	triangle_right = QPolygonF(points_right);
	//strzałki koniec

	//osie
	axis_path_origin = QPainterPath(p2);
	axis_path_origin.lineTo(p0);
	axis_path_origin.lineTo(p1);
	//osie koniec


	const int font_size = 12;
	QFont font("ISOCT_IV50");
	font.setPixelSize(font_size);
	/*font.setStyleHint(QFont::Monospace);
	font.setWordSpacing(0);
	font.setKerning(false);
	font.setItalic(false);
	font.setStyle(QFont::StyleNormal);
	font.setFixedPitch(true);
	font.setWeight(25);
*/
	//podziałka dla osi poziomej
	addScaleX( scale_x, len_x, font_size, font);
	//podziałka dla osi pionowej
	addScaleY( scale_y, len_y, font_size, font);


	axis_path_origin.addPolygon(triangle_right);
	axis_path_origin.addPolygon(triangle_up);


	//dodanie opisu osi x
	QPainterPath label_x;
	label_x.addText( ( len_x - 5*text_x.length() )/2, 3*padding[2]/5, font, text_x);

	axis_path_origin.addPath(label_x);

	//dodanie opisu osi y

	QPainterPath label_y;
	label_y.addText(0,0,font,text_y);

	for(int size = label_y.elementCount(), i = 0; i < size ; ++i)
		{
		QPainterPath::Element el = label_y.elementAt(i);

		label_y.setElementPositionAt(i,el.y,-el.x);
		}

	label_y.translate(-3*padding[3]/4, (len_y + 5*text_y.length() )/2);

	axis_path_origin.addPath(label_y);

	scaleAxis();
	}

void ScaleView::addScaleX(int scale_x, int len_x, int font_size, QFont& font)
	{
	for(int i = 0; i <= ( scale_x + 1 ); ++i)
		{
		QString str;
		std::stringstream stream;
		int x = i*len_x/(scale_x+1);
		QPointF div1(x, 0), div2(x, 3);
		double val_x = i*( getPlotWidth() - 1 )/double(scale_x+1);

		axis_path_origin.moveTo(div1);
		axis_path_origin.lineTo(div2);

		if(as_hour)
			{
			std::string s1,s2 = minToString(val_x);

			stream<<s2;
			stream>>s1>>s2;
			str = QString::fromStdString(s1+' '+s2);
			}
		else
			str = QString::number( val_x,'g',3 );

		axis_path_origin.addText( x - 5*str.length(), font_size + 5, font, str );
		}
	}

void ScaleView::addScaleY(int scale_y, int len_y, int font_size, QFont& font)
	{
	QString str;

	//podziałka dla osi pionowej
	for(int i = 0; i <= ( scale_y + 1 ); ++i)
		{
		int y = i*len_y/(scale_y+1);
		QPointF div1(0, y), div2(-3,y);
		int val_y = i*( getPlotHeigh() - 1 )/(scale_y+1);

		axis_path_origin.moveTo(div1);
		axis_path_origin.lineTo(div2);

		str = QString::number(val_y);

		axis_path_origin.addText(-font_size-7*str.length(), y + font_size/2, font, str );
		}

	}


void ScaleView::scaleAxis()
	{
	axis_path = axis_path_origin;

	point = countTranslation();

	axis_path.translate(point);
	scene->addPath(axis_path);
	}



void ScaleView::resizeEvent(QResizeEvent *event)
	{
	reScale();
	drawAxis();

	QGraphicsView::resizeEvent(event);
	}


void ScaleView::draw(const Chromosome* ch)
	{
	View::draw(ch);
	drawAxis();
	}

void ScaleView::draw(const std::vector<double>& tr)
	{
	View::draw(tr);
	drawAxis();
	}


/** ScaleView **/



void MainWindow::clearLayout(QLayout* lout)
	{
	//czyszczenie layoutu
	while(lout->count() != 0)
		{
		QLayoutItem* item = lout->itemAt(0);

		lout->removeItem(item);
		delete item->widget();
		}
	}

//slots
//początkowe współczynniki
void MainWindow::whenStartFactorsY(int yy)
    {
    y = yy;
    }

void MainWindow::whenStartFactorsA(double aa)
    {
    a = -aa;
    }

void MainWindow::whenUstawClicked()
    {
    st_factors = Factors(y,a);
    dayAction->setStartFactors(st_factors);

    updateDayActionDistplay();
    }


//kawa
void MainWindow::whenCoffeeQuantity(int qq)
    {
    coffee_quantity = qq;
    }

void MainWindow::whenCoffeeMomentChanged(QTime tt)
    {
    coffee_moment = tt;
    }

void MainWindow::whenDodajKaweClicked()
    {
    int moment =coffee_moment == QTime(0,0) ? 0 : coffee_moment.hour()*60+coffee_moment.minute();
    Coffee* action = new Coffee(moment,coffee_quantity);

    if(dayAction->addAction(action) == false)
        {
        delete action;
        return;
        }

    updateDayActionDistplay();
    }

//sen
void MainWindow::whenSleepBeginChanged(QTime tt)
    {
    sleep_begin = tt;
    }

void MainWindow::whenSleepEndChanged(QTime tt)
    {
    sleep_end = tt;

    }

void MainWindow::whenDodajSenClicked()
    {
    int begin =sleep_begin == QTime(0,0) ? 0 :sleep_begin.hour()*60+sleep_begin.minute();
    int end = sleep_end == QTime(0,0) ? 0 : sleep_end.hour()*60+sleep_end.minute();

    Sleep* action = new Sleep(begin,end);

    if(dayAction->addAction(action) == false)
        {
        delete action;
        return;
        }

    updateDayActionDistplay();
    }

//losowa
void MainWindow::whenDodajLosoweClicked()
	{
	dayAction->addRandAction();
	updateDayActionDistplay();
	}

//sprawdź i usuń przyciski
void MainWindow::whenUsunAkcjeClicked()
    {
    dayAction->removeAllActions();

    updateDayActionDistplay();
    }

void MainWindow::whenSprawdzOgraniczeniaIPoprawClicked()
    {
	dayAction->setFlagModified();

    dayAction->updateFactors();

	for(std::shared_ptr<DayActions> x : vec_DayActions)
		x->updateFactors();

    updateDayActionDistplay();
	updateStartPopulation();
    }

void MainWindow::whenDodajDoListyClicked()
	{
	Chromosome* ptr_da = dayAction->clone() ;
	std::shared_ptr<DayActions> added= std::shared_ptr<DayActions>( dynamic_cast<DayActions*>( ptr_da ) );

	if(added == nullptr)
		return;

	added->updateFactors();
	vec_DayActions.push_back(added);

	updateStartPopulation();
	}

//usuwanie predziału
void MainWindow::whenDelBegChanged(QTime tt)
    {
    del_beg = tt;
    }

void MainWindow::whenDelEndChanged(QTime tt)
    {
    del_end = tt;
    }

void MainWindow::whenDeleteRangeClicked()
    {
    int begin = del_beg == QTime(0,0) ? 0 :del_beg.hour()*60+del_beg.minute();
    int end = del_end == QTime(0,0) ? 0 : del_end.hour()*60+del_end.minute();
    TimeRange toDel(begin,end);

    dayAction->removeFromRange(toDel);

    updateDayActionDistplay();
    }

//param. statyczne

void MainWindow::whenMinCoffee(int v)
    {
    if(v > max_total_coffee)
        {
        min_coffee = max_total_coffee;
        ui->min_coffee->setValue(min_coffee);
        return;
        }

    min_coffee = v;
    }

void MainWindow::whenMaxCoffee(int v)
    {
    if(v < min_coffee)
        {
        max_total_coffee = min_coffee;
        ui->max_coffee->setValue(max_total_coffee);
        return;
        }
    max_total_coffee = v;
    ui->min_coffee->setMaximum(max_total_coffee);
    }

void MainWindow::whenYCoffeeMod(double v)
    {
    coffee_y_multiplier = v;
    }

void MainWindow::whenACoffeeMod(double v)
    {
    coffee_a_multiplier = v;
    }

void MainWindow::whenASleepMod(double v)
    {
    sleep_a_multiplier = v;
    }

void MainWindow::whenMinSleep(int v)
    {  
    min_sleep = v;
    ui->min_tot_sleep->setMinimum(min_sleep);
    }

void MainWindow::whenMinTotSleep(int v)
    {
    if(v < min_sleep)
        {
        min_total_sleep = min_sleep;
        return;
        }

    min_total_sleep  =v;
    }

void MainWindow::whenIntegerFactor(double v)
    {
    integral_multiplier = v;
    }

void MainWindow::whenCoffeeFactor(double v)
    {
    coffee_multiplier = v;
    }

void MainWindow::whenToAlgorithm() //przejscie na nastepna strone
    {
    dayAction->updateFactors();
    updateDayActionDistplay();
	std::vector<Chromosome*> vec_chrom;

	for(std::shared_ptr<DayActions> x : vec_DayActions)
		{
		x->updateFactors();
		vec_chrom.push_back(x.get());
		}

    //innicjalizacja algorytmu
	algorithm = std::make_shared<GeneticAlgorithm>(algorithm_buffor,dayAction.get(),min_difference,number_of_repeat,max_iter,max_population_size,max_best, vec_chrom);

    //przejście na następna stronę
    ui->zmianaOkna->setCurrentIndex(1);
    //wyswietlenie utworzonego wcześniej day action
    ui->stringTheBest->setText(QString::fromStdString(dayAction->toString()));
	ui->plotTheBest->draw(dayAction.get());
    }

void MainWindow::whenSetStaticFactors()
    {
    Coffee::setMinPortion(min_coffee);
    Coffee::setMaxPerDay(max_total_coffee);
    Coffee::setA_Multiplier(coffee_a_multiplier);
    Coffee::setY_Multiplier(coffee_y_multiplier);

    Sleep::setMinSleep(min_sleep);
    Sleep::setMinTotalSleep(min_total_sleep);
    Sleep::setA_Multiplier(sleep_a_multiplier);

    gF->setIntegral_Multi(integral_multiplier);
    gF->setCoffee_Multi(coffee_multiplier);

    dayAction->setFlagModified();

    dayAction->onlyUpdate();

    updateDayActionDistplay();
	updateStartPopulation();
    }


//parametry i sterowanie algorytmem
void MainWindow::whenMinDiff(double v)
    {
    algorithm->setMinDifferenceBetweenGenarationsBest(v);
    min_difference = v;
    }

void MainWindow::whenMaxIter(int v)
    {
    max_iter = static_cast<unsigned int>(v);
    algorithm->setMaxOfIteration( max_iter);
    }

void MainWindow::whenFileToSave(QString v)
    {
	file_to_save = v.toStdString();
    }

void MainWindow::whenMaxBest(int v)
    {
    max_best = static_cast<unsigned int>(v);
    algorithm->setMaxBest(max_best);
    }

void MainWindow::whenMaxPopulation(int v)
    {
    max_population_size = static_cast<unsigned int>(v);
    algorithm->setMaxPopulationSize(max_population_size);
    }

void MainWindow::whenCycleSize(int v)
    {
    cycle_length = static_cast<unsigned int>(v);
    }

void MainWindow::whenNumberOfRepeat(int v)
    {
    number_of_repeat = static_cast<unsigned int>(v);
    algorithm->setNumberOfRepeat(number_of_repeat);
    }

//przyciski save i load
void MainWindow::whenLoadAlgParamFile(QString v)
	{
	file_to_load = v.toStdString();
	}


void MainWindow::whenLoadAlgSettingsClicked()
	{
	std::fstream plik;
	plik.open(file_to_load,std::ios_base::in);

	if(plik.good())
		{
		settings_list list;
		if(loadSettings(plik,list))
			{
			min_difference = list.min_diffrence_between_generations_best;
			max_iter = list.max_of_iteration;
			max_best = list.max_best;
			max_population_size = list.max_population_size;
			number_of_repeat = list.NUMBER_OF_REPETED_SUBOPTIMAL_SOLUTION;

			algorithm->setSettings(list);

			ui->minDiff->setValue(min_difference);
			ui->maxIter->setValue(static_cast<int>(max_iter));
			ui->maxBest->setValue(static_cast<int>(max_best));
			ui->maxPopulation->setValue(static_cast<int>(max_population_size));
			ui->numberOfRepeat->setValue(static_cast<int>(number_of_repeat));
			}
		}

	plik.close();
	}

void MainWindow::whenSavePopulationClicked()
    {
	std::fstream plik;
	std::string nazwa = file_to_save + (".pop");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		for(const Chromosome* x : *( algorithm->getPopulation() ) )
			plik<<(*x);
		}

	plik.close();
    }

void MainWindow::whenSaveBestClicked()
    {
	std::fstream plik;
	std::string nazwa = file_to_save + (".da");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		plik<<(algorithm->getTheBest()->toString());
		}

	plik.close();
    }

void MainWindow::whenSaveAlgorithmParametersClicked()
	{
	std::fstream plik;
	std::string nazwa = file_to_save + (".alg.set");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		settings_list list = algorithm->getSettings();
		plik<< list;
		}

	plik.close();
    }

void MainWindow::whenSaveStatusClicked()
    {
	std::fstream plik;
	std::string nazwa = file_to_save + (".status");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		algorithm->status(plik);
		}

	plik.close();
    }

//sterowanie
void MainWindow::whenStartButtonClicked()
	{
    //start algorytmu
	ui->startButton->setEnabled(false); //wyłączenie przyciku start

    setAlgorithmParameters();

    //start algorytmu
	best = std::shared_ptr<Chromosome>(algorithm->startAlgorithm(false,cycle_length));

	if(!algorithm->isEnd())
		ui->continueButton->setEnabled(true);

	std::vector<double> tr = algorithm->getValueTrace();

	ui->trace->draw(tr);

    updateDisplaying();
    }

void MainWindow::whenResetButtonClicked()
    {
    //restart
	std::vector<Chromosome*> vec_chrom;

	for(std::shared_ptr<DayActions> x : vec_DayActions)
		vec_chrom.push_back(x.get());

	algorithm->restart(dayAction.get(),min_difference,number_of_repeat,max_iter,max_population_size,max_best,vec_chrom);
    ui->startButton->setEnabled(true); //aktywacja startu i dezaktywacja kontynuacji
    ui->continueButton->setEnabled(false);

    updateDisplaying();
    ui->stringTheBest->setText(QString::fromStdString(dayAction->toString()));
	ui->plotTheBest->draw(dayAction.get());


	std::vector<double> tr = algorithm->getValueTrace();

	ui->trace->draw(tr);

    //czyszczenie layoutu
    clearLayout(ui->verticalLayout_15);
    }

void MainWindow::whenContinueButtonClicked()
    {
    //kontynuowanie algorytmu
    setAlgorithmParameters();

    //start algorytmu
    best = std::shared_ptr<Chromosome>(algorithm->resume(false,cycle_length));


	std::vector<double> tr = algorithm->getValueTrace();

	ui->trace->draw(tr);

    if(!algorithm->isEnd())
        ui->continueButton->setEnabled(true);
    else
        {
        ui->continueButton->setEnabled(false);
		}


    updateDisplaying();
    }

void MainWindow::whenChangeStartObjectButton()
    {
    best = nullptr;
    algorithm = nullptr;

    ui->startButton->setEnabled(true);
    ui->continueButton->setEnabled(false);

    ui->zmianaOkna->setCurrentIndex(0);
    }

void MainWindow::whenQuitButton()
    {
    sleep = nullptr;
    coffee = nullptr;
    vec.clear();
    fun.clear();
    gF = nullptr;
    dayAction =nullptr;
    algorithm = nullptr;
    best = nullptr;

    emit quit();
    }



//odczyt i zapis do pliku page 1
void MainWindow::whenSaveFile(QString v)
	{
	plik_output = v.toStdString();
	}

void MainWindow::whenLoadFile(QString v)
	{
	plik_input = v.toStdString();
	}


void MainWindow::whenLoadStartObjectClicked()
	{
	std::fstream plik;
	plik.open(plik_input,std::ios_base::in);

	if(plik.good())
		{
		std::shared_ptr<DayActions> da = DayActions::createFromFile(plik,parse_form,gF,vec,fun);

		if(da !=nullptr)
			{
			dayAction = da;
			updateDayActionDistplay();
			}
		}

	plik.close();
	}

void MainWindow::whenLoadStartPopulationClicked()
	{
	std::fstream plik;
	plik.open(plik_input,std::ios_base::in);

	if(plik.good())
		{
		std::vector< std::shared_ptr<DayActions>> vec1 = loadFromFile(plik,parse_form,gF,vec,fun);
		if(vec1.size() != 0)
			{
			vec_DayActions = vec1;
			updateStartPopulation();
			}
		}

	plik.close();
	}

void MainWindow::whenLoadStaticParametersClicked()
	{
	std::fstream plik;
	plik.open(plik_input,std::ios_base::in);

	if(plik.good())
		fromFileStaticParam(plik);


	plik.close();
	}


void MainWindow::whenSaveStartObjectClicked()
	{

	std::fstream plik;
	std::string nazwa = plik_output + (".da");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		if(dayAction != nullptr)
			{
			plik<<(*dayAction);
			}
		}

	plik.close();
	}

void MainWindow::whenSaveStartPopulationClicked()
	{

	std::fstream plik;
	std::string nazwa = plik_output + (".pop");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		for(std::shared_ptr<DayActions> x : vec_DayActions )
			plik<<(*x);
		}

	plik.close();
	}

void MainWindow::whenSaveStaticParametersClicked()
	{
	std::fstream plik;
	std::string nazwa = plik_output + ".static.param";
	plik.open(nazwa,std::ios_base::out);

	if(plik.good())
		{
		plik<<staticParamToString();
		}

	plik.close();
	}


std::string MainWindow::staticParamToString()
	{
	std::stringstream str;
	str<<min_coffee<<"\n"<<max_total_coffee<<"\n"<<
		 coffee_a_multiplier<<"\n"<<coffee_y_multiplier<<"\n"<<
		 min_sleep<<"\n"<<min_total_sleep<<"\n"<<
		 sleep_a_multiplier<<"\n"<<integral_multiplier<<"\n"<<
		 coffee_multiplier<<"\n";
	return  str.str();
	}


bool MainWindow::fromFileStaticParam(std::istream& in)
	{
	int m1,m2,m5,m6;
	double m3,m4,m7,m8,m9;

	if(in.bad() || !( in>>m1 ) || !( in>>m2 ) || !( in>>m3 ) || !( in>>m4 ) || !( in>>m5 ) || !( in>>m6 ) ||
	   !( in>>m7 ) || !( in>>m8 ) || !( in>>m9 ) )
	   return false;

	min_coffee = m1;
	max_total_coffee = m2;
	coffee_a_multiplier = m3;
	coffee_y_multiplier = m4;
	min_sleep = m5;
	min_total_sleep = m6;
	sleep_a_multiplier = m7;
	integral_multiplier = m8;
	coffee_multiplier = m9;

	updateDisplayStaticParam();

	return true;
	}


void MainWindow::updateDisplayStaticParam()
	{
	ui->min_coffee->setMaximum(max_total_coffee);
	ui->min_coffee->setValue(min_coffee);

	ui->max_coffee->setMinimum(min_coffee);
	ui->max_coffee->setValue(max_total_coffee);

	ui->a_coffee_mod->setValue(coffee_a_multiplier);
	ui->y_coffee_mod->setValue(coffee_y_multiplier);

	ui->min_sleep->setValue(min_sleep);
	ui->min_tot_sleep->setValue(min_total_sleep);
	ui->a_sleep_mod->setValue(sleep_a_multiplier);

	ui->integer_factor->setValue(integral_multiplier);
	ui->coffee_factor->setValue(coffee_multiplier);

	whenSetStaticFactors();
	}

void MainWindow::whenSaveGoalTraceClicked()
	{
	std::fstream plik;
	std::string nazwa = file_to_save + (".trace");
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		std::vector<double> trace;
		trace = algorithm->getValueTrace();

		for(double x : trace)
			plik<<x<<std::endl;
		}

	plik.close();


	}


