// Qt classes used to build the graphical interface and display messages
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapwidget.h"
#include <QVBoxLayout>
#include <QMessageBox>

// libraries for file handling and data storage
#include <fstream>
#include <vector>
#include <set>

// stores the information about a previously simulated emergency
// each record contains the starting room and the blocked location
struct emergency_record {
    int startRoomId;
    int blockedLocationId;
};

// appends a new emergency record to emergencies.txt
// it preserves previously recorded emergencies
void save_emergency(int startRoomId, int blockedLocationId)
{
    std::ofstream file("emergencies.txt", std::ios::app);

    if (!file.is_open()) {
        return;
    }

    file << startRoomId << " " << blockedLocationId << std::endl;
    file.close();
}

// reads the saved emergency records from the file and returns them in a vector
std::vector<emergency_record> load_emergencies()
{
    std::vector<emergency_record> emergencies;

    std::ifstream file("emergencies.txt");

    if (!file.is_open()) {
        return emergencies;
    }

    emergency_record emergency;

    while (file >> emergency.startRoomId >> emergency.blockedLocationId) {
        emergencies.push_back(emergency);
    }

    file.close();

    return emergencies;
}

// stores the map information that must be restored when the application is reopened
struct map_state {
    int selectedRoomId = 1;
    std::set<int> blockedHallways;
    std::set<int> blockedExits;
};

// saves the selected room and all currently blocked hallways and exits to map_state.txt
void save_map_state(int selectedRoomId,
    const std::set<int>& blockedHallways,
    const std::set<int>& blockedExits)
{
    std::ofstream file("map_state.txt");

    if (!file.is_open())
        return;

    file << selectedRoomId << '\n';

    file << blockedHallways.size() << '\n';
    for (int id : blockedHallways)
        file << id << '\n';

    file << blockedExits.size() << '\n';
    for (int id : blockedExits)
        file << id << '\n';
}

// loads and validates the saved map information
// temporary sets are used so the current state is only updated after all saved values have been read successfully
bool load_map_state(map_state& state)
{
    std::ifstream file("map_state.txt");

    if (!file.is_open())
        return false;

    int roomId;
    int hallwayCount;
    int exitCount;

    if (!(file >> roomId) || roomId < 1 || roomId > 12)
        return false;

    if (!(file >> hallwayCount) || hallwayCount < 0 || hallwayCount > 4)
        return false;

    std::set<int> hallways;
    std::set<int> exits;

    for (int i = 0; i < hallwayCount; i++)
    {
        int id;
        if (!(file >> id) || id < 101 || id > 104)
            return false;

        hallways.insert(id);
    }

    if (!(file >> exitCount) || exitCount < 0 || exitCount > 2)
        return false;

    for (int i = 0; i < exitCount; i++)
    {
        int id;
        if (!(file >> id) || id < 201 || id > 202)
            return false;

        exits.insert(id);
    }

    state.selectedRoomId = roomId;
    state.blockedHallways = hallways;
    state.blockedExits = exits;

    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // initializes the main window and sets the initial application status
    ui->setupUi(this);
    updateStatus("Ready");
    
    // builds the building's graph by connecting hallways, rooms, and exits with their distances
    // the Pathfinder.h uses these connections to calculate evacuation routes
    
    // hallways
    pathfinder.addconnections(101, 102, 20.0);
    pathfinder.addconnections(101, 104, 25.0);
    pathfinder.addconnections(102, 103, 20.0);
    pathfinder.addconnections(103, 104, 25.0);

    // connects each room to the hallway assigned to its section of the building
    for (int roomId = 1; roomId <= 12; roomId++)
    {
        int hallwayId;

        if (roomId <= 4)
            hallwayId = 101;
        else if (roomId <= 6)
            hallwayId = 102;
        else if (roomId <= 10)
            hallwayId = 103;
        else
            hallwayId = 104;

        pathfinder.addconnections(roomId, hallwayId, 10.0);
    }

    // connects Exit 201 to Hallway 101 and Exit 202 to Hallway 103
    pathfinder.addconnections(101, 201, 3.0);
    pathfinder.addconnections(103, 202, 3.0);
    
    // associates each dropdown item with its actual hallway or exit ID
    ui->hallwayComboBox->setItemData(0, 101);
    ui->hallwayComboBox->setItemData(1, 102);
    ui->hallwayComboBox->setItemData(2, 103);
    ui->hallwayComboBox->setItemData(3, 104);
    ui->exitComboBox->setItemData(0, 201);
    ui->exitComboBox->setItemData(1, 202);

    // sets readable labels for the hallway and exit selection menus
    ui->hallwayComboBox->setItemText(0, "Hallway 101");
    ui->hallwayComboBox->setItemText(1, "Hallway 102");
    ui->hallwayComboBox->setItemText(2, "Hallway 103");
    ui->hallwayComboBox->setItemText(3, "Hallway 104");
    ui->exitComboBox->setItemText(0, "Exit 201");
    ui->exitComboBox->setItemText(1, "Exit 202");

    // connects the GUI buttons to their corresponding event handler function
    // when a button is clicked, Qt calls the associated function in MainWindow
    connect(ui->findRouteButton, &QPushButton::clicked,
            this, &MainWindow::onFindRouteClicked);

    connect(ui->blockHallwayButton, &QPushButton::clicked,
            this, &MainWindow::onBlockHallwayClicked);

    connect(ui->blockExitButton, &QPushButton::clicked,
            this, &MainWindow::onBlockExitClicked);

    connect(ui->compareExitsButton, &QPushButton::clicked,
            this, &MainWindow::onCompareExitsClicked);

    connect(ui->usedEmergenciesButton, &QPushButton::clicked,
            this, &MainWindow::onUsedEmergenciesClicked);

    // creates the custom map widget and connects it to the Building object
    // the widget is responsible for displaying the building map
    mapWidget = new MapWidget(building, ui->mapWidget);
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // adds the custom map widget to the map area of the main window
    QVBoxLayout *mapLayout = new QVBoxLayout(ui->mapWidget);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    mapLayout->addWidget(mapWidget);

    // attempts to restore the previously saved map configuration when the application starts
    map_state savedState;

    if (load_map_state(savedState))
    {
        ui->findRouteButton_2->setCurrentIndex(savedState.selectedRoomId - 1);

        // restores the blocked hallways and exits in the map widget
        for (int hallwayId : savedState.blockedHallways)
            mapWidget->setHallwayBlocked(hallwayId, true);

        for (int exitId : savedState.blockedExits)
            mapWidget->setExitBlocked(exitId, true);

        // restores the selected room on the map
        mapWidget->setSelectedRoom(savedState.selectedRoomId);

        // recalculates the evacuation route using the restored room and blocked locations
        path_results results = pathfinder.calculate_distances(
            savedState.selectedRoomId,
            -1,
            -1,
            savedState.blockedExits,
            savedState.blockedHallways
        );

        // displays the restored route if one is available
        // otherwise, clears the route from the map.
        if (results.found)
            mapWidget->setRoute(results.route);
        else
            mapWidget->setRoute({});
    }
    // save selected room whenever it changes
    connect(ui->findRouteButton_2,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int index)
        {
            save_map_state(
                index + 1,
                mapWidget->getBlockedHallways(),
                mapWidget->getBlockedExits()
            );
        });
}

// updates the status label in the main window to provide feedback about the current application state
void MainWindow::updateStatus(const QString& message)
{
    ui->statusLabel->setText("Status: " + message);
}

// handles the Find Route button
// gets the selected room and blocked locations from the GUI,
// sends this information to the Pathfinder, and displays the result
void MainWindow::onFindRouteClicked()
{
    // gets the room selected by the user
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;
    
    // updates the selected room displayed on the map
    mapWidget->setSelectedRoom(selectedRoomId);

    std::set<int> blockedExitIds;
    std::set<int> blockedHallwayIds;

    // collects the IDs of all currently blocked exits and hallways
    
    // check blocked exits
    if (mapWidget->isExitBlocked(201))
        blockedExitIds.insert(201);

    if (mapWidget->isExitBlocked(202))
        blockedExitIds.insert(202);

    // check blocked hallways
    for (int hallwayId = 101; hallwayId <= 104; hallwayId++)
    {
        if (mapWidget->isHallwayBlocked(hallwayId))
            blockedHallwayIds.insert(hallwayId);
    }

    // calls the Pathfinder to calculate the nearest available
    // evacuation route while considering blocked locations
    path_results results =
        pathfinder.calculate_distances(
            selectedRoomId,      // starting room ID
            -1,                  // no specific hallway destination
            -1,                  // no specific exit destination
            blockedExitIds,      // exits that are currently blocked
            blockedHallwayIds    // hallways that are currently blocked
        );
    
    // handles the case where no valid evacuation route is available
    if (!results.found)
    {
        // clears the displayed route and informs the user
        mapWidget->setRoute({});
        updateStatus("No Available Route");
        QMessageBox::warning(
            this,
            "No Route Available",
            "No evacuation route is available from this room."
        );
        return;
    }
    
    // sends the calculated route to MapWidget for visualization
    mapWidget->setRoute(results.route);
    
    // updates the status label in the main window to provide feedback about the current application state
    updateStatus("Route Found");
    
    // converts the route's room, hallway, and exit IDs
    // into a readable sequence for the result message
    QString routeText;

    for (size_t i = 0; i < results.route.size(); i++)
    {
        routeText += QString::number(results.route[i]);

        if (i < results.route.size() - 1)
            routeText += " -> ";
    }

    QString message =
        "Nearest Exit: Exit " + QString::number(results.exitid)
        + "\nRoute: " + routeText
        + "\nTotal Distance: "
        + QString::number(results.total_distance) + " meters";

    QMessageBox::information(this, "Evacuation Route",message);
}

// handles the hallway blocking button
// updates the hallway status, saves the map state,
// and records newly created emergency situations
void MainWindow::onBlockHallwayClicked()
{
    // gets the actual hallway ID selected in the dropdown menu
    int selectedHallwayId =
        ui->hallwayComboBox->currentData().toInt();

    // checks whether the selected hallway is already blocked
    bool currentlyBlocked =
        mapWidget->isHallwayBlocked(selectedHallwayId);

    // toggles the hallway status and updates the map
    mapWidget->setHallwayBlocked(selectedHallwayId, !currentlyBlocked);
    
    // gets the currently selected room ID
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;

    // saves the updated map configuration
    save_map_state(
        selectedRoomId,
        mapWidget->getBlockedHallways(),
        mapWidget->getBlockedExits()
    );

    // records the emergency only when a hallway becomes blocked
    if (!currentlyBlocked)
    {
        save_emergency(selectedRoomId, selectedHallwayId);
    }
    
    // updates the application status
    if (currentlyBlocked)
        updateStatus("Hallway Unblocked");
    else
        updateStatus("Hallway Blocked");

    QString message;

    if (currentlyBlocked)
    {
        message = "Hallway "
                  + QString::number(selectedHallwayId)
                  + " has been unblocked.";
    }
    else
    {
        message = "Hallway "
                  + QString::number(selectedHallwayId)
                  + " has been blocked.";
    }

    QMessageBox::information(
        this,
        "Hallway Status",
        message
        );
}

// handles the exit blocking button
// updates the exit status, saves the map state and records newly created emergency situations
void MainWindow::onBlockExitClicked()
{
    // gets the actual exit ID selected in the dropdown menu
    int selectedExitId = ui->exitComboBox->currentData().toInt();

    // checks whether the selected exit is already blocked
    bool currentlyBlocked = mapWidget->isExitBlocked(selectedExitId);

    // toggles the exit status and updates the map
    mapWidget->setExitBlocked(selectedExitId, !currentlyBlocked);
    
    // gets the currently selected room ID
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;

    // saves the updated map configuration
    save_map_state(
        selectedRoomId,
        mapWidget->getBlockedHallways(),
        mapWidget->getBlockedExits()
    );

    // records the emergency only when an exit becomes blocked
    if (!currentlyBlocked)
    {
        save_emergency(selectedRoomId, selectedExitId);
    }
    
    // updates the application status
    if (currentlyBlocked)
        updateStatus("Exit Unblocked");
    else
        updateStatus("Exit Blocked");
    QString message;

    if (currentlyBlocked)
    {
        message = "Exit " + QString::number(selectedExitId)
        + " has been unblocked.";
    }
    else
    {
        message = "Exit " + QString::number(selectedExitId)
        + " has been blocked.";
    }
    // displays the result to the user.
    QMessageBox::information(this, "Exit Status", message);
}

// handles the Compare Exits button
// calculates the distance from the selected room to each exit and displays whether each exit is available
void MainWindow::onCompareExitsClicked()
{
    // gets the room selected by the user
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;

    std::set<int> blockedExitIds;
    std::set<int> blockedHallwayIds;

    // collects the IDs of all currently blocked exits
    if (mapWidget->isExitBlocked(201))
        blockedExitIds.insert(201);

    if (mapWidget->isExitBlocked(202))
        blockedExitIds.insert(202);

    // collects the IDs of all currently blocked hallways
    for (int hallwayId = 101; hallwayId <= 104; hallwayId++)
    {
        if (mapWidget->isHallwayBlocked(hallwayId))
            blockedHallwayIds.insert(hallwayId);
    }

    // calculates the route specifically to Exit 201
    path_results exit1 =
        pathfinder.calculate_distances(
            selectedRoomId, -1, 201,
            blockedExitIds, blockedHallwayIds
        );

    // calculates the route specifically to Exit 202
    path_results exit2 =
        pathfinder.calculate_distances(
            selectedRoomId, -1, 202,
            blockedExitIds, blockedHallwayIds
        );

    QString message;

    // adds the distance to Exit 201 or indicates that it is unavailable
    if (exit1.found)
        message += "Exit 201: " + QString::number(exit1.total_distance) + " meters\n";
    else
        message += "Exit 201: Unavailable\n";

    // adds the distance to Exit 202 or indicates that it is unavailable
    if (exit2.found)
        message += "Exit 202: " + QString::number(exit2.total_distance) + " meters\n";
    else
        message += "Exit 202: Unavailable\n";

    // updates the application status
    updateStatus("Exits Compared");

    // displays the comparison results to the user
    QMessageBox::information(
        this,
        "Compare Exits",
        message
    );
}

// handles the Used Emergencies button
// loads previously recorded emergency situations
// and displays their starting rooms and blocked locations
void MainWindow::onUsedEmergenciesClicked()
{
    std::vector<emergency_record> emergencies = load_emergencies();

    // informs the user when there are no saved emergency records
    if (emergencies.empty())
    {
        QMessageBox::information(
            this,
            "Used Emergencies",
            "No previous emergencies found."
        );
        return;
    }

    QString message;

    // goes through each saved emergency and prepares its information for display
    for (const emergency_record& emergency : emergencies)
    {
        message += "Starting Room: "
            + QString::number(emergency.startRoomId);

        if (emergency.blockedLocationId >= 101 &&
            emergency.blockedLocationId <= 104)
        {
            message += "\nBlocked Hallway: "
                + QString::number(emergency.blockedLocationId);
        }
        else
        {
            message += "\nBlocked Exit: "
                + QString::number(emergency.blockedLocationId);
        }

        message += "\n\n";
    }

    // displays the saved emergency records to the user
    QMessageBox::information(
        this,
        "Used Emergencies",
        message
    );
}

// releases the memory allocated for the MainWindow UI
MainWindow::~MainWindow()
{
    delete ui;
}
