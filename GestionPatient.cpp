#include<iostream>
#include<list>
#include <string>
#include <cstdio>
#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "";

class connectSql {
	Driver* driver;
	Connection* con;
	Statement* stmt;
	PreparedStatement* pstmt;
	ResultSet* res;
public:
	void Connect() {
		try
		{
			driver = get_driver_instance();
			con = driver->connect(server, username, password);
		}
		catch (SQLException e)
		{
			cout << "Connexion refusée au serveur distant. Message d'erreur : " << e.what() << endl;
			system("pause");
			exit(1);
		}
		con->setSchema("miniprojetcpp");
	}

	void PurgeDb()
	{
		stmt = con->createStatement();

		stmt->execute("DROP TABLE IF EXISTS patient");
		cout << "Finished dropping table patient (if existed)" << endl;

		stmt->execute("DROP TABLE IF EXISTS vaccin");
		cout << "Finished dropping table vaccin (if existed)" << endl;

		stmt->execute("CREATE TABLE patient (id serial, nom VARCHAR(50), prenom VARCHAR(50), sexe VARCHAR(50), CIN VARCHAR(50) PRIMARY KEY, age INTEGER);");
		cout << "Finished creating table patient" << endl;

		stmt->execute("CREATE TABLE vaccin (id serial PRIMARY KEY, CIN VARCHAR(50), type VARCHAR(50), date_vaccin VARCHAR(50), Numero_vaccin INTEGER);");
		cout << "Finished creating table vaccin" << endl;

		delete stmt;

	}
	void AddPatient(string nom, string prenom, int age, string sexe, string CIN)
	{
		pstmt = con->prepareStatement("INSERT INTO patient(nom, prenom, sexe, CIN, age) VALUES(?,?,?,?,?)");
		pstmt->setString(1, nom);
		pstmt->setString(2, prenom);
		pstmt->setString(3, sexe);
		pstmt->setString(4, CIN);
		pstmt->setInt(5, age);
		pstmt->execute();
		cout << "Une ligne a ete ajoute." << endl;
		delete pstmt;
	}
	void GetPatients()
	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * from patient");
		cout << "ID\tNom\t\t\tPrenom\t\t\tSexe\t\t\tCIN\t\tAge" << endl;
		while (res->next()) {
			cout << res->getInt("id") << "\t"
				<< res->getString("nom") << "\t\t\t"
				<< res->getString("prenom") << "\t\t\t"
				<< res->getString("sexe") << "\t\t"
				<< res->getString("CIN") << "\t\t"
				<< res->getInt("age") << endl;
		}
		cout << "" << endl;
		delete stmt;
	}
	bool GetPatientByCIN(string CIN)
	{
		try {
			pstmt = con->prepareStatement("SELECT * from patient WHERE CIN=?");
			pstmt->setString(1, CIN);
			res = pstmt->executeQuery();
			if (res->next()) {
				// Affichage du patient trouvé
				cout << "CIN : " << res->getString("CIN") << endl;
				cout << "Nom : " << res->getString("nom") << endl;
				cout << "Prenom : " << res->getString("prenom") << endl;
				cout << "Sexe : " << res->getString("sexe") << endl;
				cout << "Age : " << res->getInt("age") << endl;

				return true;
			}
			else {
				cout << "La base de donnee ne contient aucun patient avec le CIN : `" << CIN << "`." << endl;
				return false;
			}
			cout << "" << endl;
			delete pstmt;
		}
		catch (sql::SQLException& e) {
			cerr << "Erreur MySQL : " << e.what() << endl;
		}
	}
	void ModPatient(string CIN, string nom, string prenom, int age, string sexe) {
		pstmt = con->prepareStatement("UPDATE patient SET nom=?, prenom=?, age=?, sexe=? WHERE CIN=?");
		pstmt->setString(1, nom);
		pstmt->setString(2, prenom);
		pstmt->setInt(3, age);
		pstmt->setString(4, sexe);
		pstmt->setString(5, CIN);
		pstmt->execute();
		cout << "Une ligne modifiée." << endl;
		delete pstmt;
	}
	void SupPatient(string CIN) {
		pstmt = con->prepareStatement("DELETE FROM patient WHERE CIN=?");
		pstmt->setString(1, CIN);
		pstmt->execute();
		cout << "Une ligne supprimée." << endl;
		delete pstmt;
	}
	void AddVaccin(string CIN, string type, string date_vaccin, int Numero_vaccin)
	{
		pstmt = con->prepareStatement("INSERT INTO vaccin(CIN, type, date_vaccin, Numero_vaccin) VALUES(?,?,?,?)");
		pstmt->setString(1, CIN);
		pstmt->setString(2, type);
		pstmt->setString(3, date_vaccin);
		pstmt->setInt(4, Numero_vaccin);
		pstmt->execute();
		cout << "Une ligne ajoutée." << endl;
		delete pstmt;
	}
	bool GetVaccinByCIN(string CIN)
	{
		try {
			pstmt = con->prepareStatement("SELECT * from vaccin WHERE CIN=?");
			pstmt->setString(1, CIN);
			res = pstmt->executeQuery();
			if (res->next()) {
				// Affichage du vaccin trouvé
				cout << "Identifiant technique : " << res->getString("id") << endl;
				cout << "CIN : " << res->getString("CIN") << endl;
				cout << "Type de vaccin : " << res->getString("type") << endl;
				cout << "Date vaccination : " << res->getString("date_vaccin") << endl;
				cout << "Num vaccin : " << res->getInt("Numero_vaccin") << endl;

				return true;
			}
			else {
				cout << "La base de donnee ne contient aucun patient avec le CIN : `" << CIN << "`." << endl;
				return false;
			}
			cout << "" << endl;
			delete pstmt;
		}
		catch (sql::SQLException& e) {
			cerr << "Erreur MySQL : " << e.what() << endl;
		}
	}
	bool GetVaccinById(string id)
	{
		try {
			pstmt = con->prepareStatement("SELECT * from vaccin WHERE id=?");
			pstmt->setString(1, id);
			res = pstmt->executeQuery();
			if (res->next()) {
				// Affichage du vaccin trouvé
				cout << "Identifiant technique : " << res->getString("id") << endl;
				cout << "CIN : " << res->getString("CIN") << endl;
				cout << "Type de vaccin : " << res->getString("type") << endl;
				cout << "Date vaccination : " << res->getString("date_vaccin") << endl;
				cout << "Num vaccin : " << res->getInt("Numero_vaccin") << endl;

				return true;
			}
			else {
				cout << "La base de donnee ne contient aucun vaccin avec l'id : `" << id << "`." << endl;
				return false;
			}
			cout << "" << endl;
			delete pstmt;
		}
		catch (sql::SQLException& e) {
			cerr << "Erreur MySQL : " << e.what() << endl;
		}
	}
	void GetVaccins()
	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * from vaccin");
		cout << "ID\tCIN\t\t\tType vaccin\t\t\tDate vaccination\t\tNum vaccin" << endl;
		while (res->next()) {
			cout << res->getInt("id") << "\t"
				<< res->getString("CIN") << "\t\t\t"
				<< res->getString("type") << "\t\t\t\t"
				<< res->getString("date_vaccin") << "\t\t\t"
				<< res->getString("Numero_vaccin") << endl;
		}
		cout << "" << endl;
		delete stmt;
	}
	int GetNbrVaccinations()
	{
		int count;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT count(*) count from vaccin");
		while (res->next()) {
			count = res->getInt("count");
		}
		delete stmt;
		return count;
	}
	int GetNbrPremiereDoses()
	{
		int nombre_doses_non_dupliques;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT COUNT(DISTINCT type) AS nombre_doses_non_dupliques FROM vaccin");
		while (res->next()) {
			nombre_doses_non_dupliques = res->getInt("nombre_doses_non_dupliques");
		}
		delete stmt;
		return nombre_doses_non_dupliques;
	}
	int GetNbrScndDose()
	{
		int nombre_secondes_doses = 0;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT CIN FROM vaccin GROUP BY CIN HAVING COUNT(*) > 1");
		// Récupération du nombre total de CIN dupliqués
		while (res->next()) {
			nombre_secondes_doses++;
		}
		delete stmt;
		return nombre_secondes_doses;
	}
	void ModVaccin(string CIN, string type, string date_vaccin) {
		pstmt = con->prepareStatement("UPDATE vaccin SET type=?, date_vaccin=? WHERE CIN=?");
		pstmt->setString(3, CIN);
		pstmt->setString(1, type);
		pstmt->setString(2, date_vaccin);
		pstmt->execute();
		cout << "Une ligne modifiée." << endl;
		delete pstmt;
	}
	void SupVaccin(string id) {
		pstmt = con->prepareStatement("DELETE FROM vaccin WHERE id=?");
		pstmt->setString(1, id);
		pstmt->execute();
		cout << "Une ligne supprimée." << endl;
		delete pstmt;
	}
	void Dispose()
	{
		delete res;
		delete con;
	}
};
connectSql sqlCon;
class Patient {
public:
	string CIN;
	string nom;
	string prenom;
	string sexe;
	int age;
	Patient* suivantP;
	Patient(string n, string p, int a, string s, string c) {
		nom = n;
		age = a;
		sexe = s;
		CIN = c;
		prenom = p;
		suivantP = NULL;
	}
};
class Vaccin {
protected:
	int Numero_vaccin;
	bool premier_dose;
public:
	string type;
	string date_vaccin;
	string CIN;
	Vaccin* suivantV;
	Vaccin(string c, string t, string d, int Numero_vaccin) {
		CIN = c;
		type = t;
		date_vaccin = d;
		this->Numero_vaccin = Numero_vaccin;
		suivantV = NULL;
	}
	Vaccin(int Numero_vaccin, bool premier_dose) {
		this->Numero_vaccin = Numero_vaccin;
		this->premier_dose = premier_dose;
	}
	int getNumVaccine() const {
		return Numero_vaccin;
	}
	bool getIsFirstDose() const {
		return premier_dose;
	}
};
class GestionPatient {
private:
	Patient* premier_Patient;
	Vaccin* premier_vaccin;
	list<Vaccin> ListVaccins;
	list<Patient> ListPatients;

	vector<Vaccin> vaccinations;

public:
	GestionPatient() {
		premier_Patient = NULL;
		premier_vaccin = NULL;
	}
	void AjouterPatient(string nom, string prenom, int age, string sexe, string CIN) {
		// Legacy code
		/*Patient* nouveauPatient = new Patient(nom, prenom, age, sexe, CIN);
		if (premier_Patient == NULL) {
			premier_Patient = nouveauPatient;
		}
		else {
			Patient* PatientCourant = premier_Patient;
			while (PatientCourant->suivantP != NULL) {
				PatientCourant = PatientCourant->suivantP;
			}
			PatientCourant->suivantP = nouveauPatient;
		}*/

		// Mysql Code
		sqlCon.AddPatient(nom, prenom, age, sexe, CIN);
	}

	void AfficherPatient() {
		//Patient* patientCourant = premier_Patient;
		sqlCon.GetPatients();
	}
	void ModdifierPatient(string CIN) {
		bool found = sqlCon.GetPatientByCIN(CIN);
		if (found)
		{
			string nouveauNom, nouveauPrenom, nouveauSexe;
			int nouvelAge;
			cout << "=> Entrez le nouveau nom    : ";
			cin >> nouveauNom;
			cout << "=> Entrez le nouveau prenom : ";
			cin >> nouveauPrenom;
			cout << "=> Entrez le nouvel âge     : ";
			cin >> nouvelAge;
			cout << "=> Entrez le nouveau sexe   : ";
			cin >> nouveauSexe;
			sqlCon.ModPatient(CIN, nouveauNom, nouveauPrenom, nouvelAge, nouveauSexe);
			cout << "                                           " << endl;
			cout << "Patient modifie avec succes √ ." << endl;
		}
	}
	void supprimerPatient(string CIN) {
		if (sqlCon.GetPatientByCIN(CIN)) {
			sqlCon.SupPatient(CIN);
			cout << "                                           " << endl;
			cout << "Le patient est supprime avec succes √ ." << endl;
		}
		return;
	}
	void AjouterVaccin(string CIN, string type, string date_vaccin, int Numero_vaccin) {
		sqlCon.AddVaccin(CIN, type, date_vaccin, Numero_vaccin);
	}
	void Ajouter_statistique_vaccin(Vaccin v) {
		vaccinations.push_back(v);
	}
	void AfficherVaccin() {
		sqlCon.GetVaccins();
	}
	void ModdifierVaccin(string CIN) {
		if (sqlCon.GetVaccinByCIN(CIN))
		{
			string nouveauType, nouveauDate;
			cout << "=> Entrez le nouveau type    : ";
			cin >> nouveauType;
			cout << "=> Entrez la nouvelle date de vaccination : ";
			cin >> nouveauDate;
			sqlCon.ModVaccin(CIN, nouveauType, nouveauDate);
			cout << "                                           " << endl;
			cout << "Vaccin modifié avec succès √ ." << endl;
		}
	}
	void SupprimerVaccin(string id) {
		if (sqlCon.GetVaccinById(id))
		{
			string rep;
			cout << "=> Confirmer la suppression y/n : ";
			cin >> rep;
			if (rep == "y")
			{
				sqlCon.SupVaccin(id);
				cout << "                                           " << endl;
				cout << "Le vaccin est supprimé avec succes √ ." << endl;
			}
		}
		return;
	}
	int getNombreVaccinations() const {
		return vaccinations.size();
	}
	int getNumPremieresDoses() const {
		int count = 0;
		for (int i = 0; i < vaccinations.size(); i++) {
			if (vaccinations[i].getIsFirstDose()) {
				count++;
			}
		}
		return count;
	}
	int getNumSecondesDoses() const {
		int count = 0;
		for (int i = 0; i < vaccinations.size(); i++) {
			if (!vaccinations[i].getIsFirstDose()) {
				count++;
			}
		}
		return count;
	}
};
class VaccinationStatistics : public GestionPatient {
public:
	double getPourcentagePremieresDoses() const {
		return getNumPremieresDoses() / static_cast<double>(getNombreVaccinations()) * 100.0;
	}
	double getPourcentageSecondesDoses() const {
		return getNumSecondesDoses() / static_cast<double>(getNombreVaccinations()) * 100.0;
	}
};
int main() {
	GestionPatient ListPatients;
	GestionPatient ListVaccins;
	string continu = "y";
	int choix;
	do {
		sqlCon.Connect();

		cout << "##########################################################################" << endl;
		cout << ".............................Menu........................................." << endl;
		cout << "1.........Ajouter Patient      |     5.........Ajouter Vaccin" << endl;
		cout << "2.........Afficher patient     |     6.........Afficher Vaccin" << endl;
		cout << "3.........Moddifier Patient    |     7.........Modifier Vaccin" << endl;
		cout << "4.........Supprimer Patient    |     8.........Supprimer Vaccin" << endl;
		cout << "9........Afficher STQ Vaccin" << endl;
		cout << "                              ***" << endl;
		cout << "0.........Initialiser la BDD   |     99........Exit" << endl;
		cout << "" << endl;
		cout << "##########################################################################" << endl;
		cout << "=> Entrer votre choix   1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 0 _ 99  " << endl;
		cout << "##########################################################################" << endl;
		cin >> choix;
		switch (choix)
		{
		case 0: {
			sqlCon.PurgeDb();
			break;
		}
		case 1: {
			string nom, sexe, prenom, CIN;
			int age;
			cout << "                 Ajouter Patient" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez le nom du patient    : ";
			cin >> nom;
			cout << "=> Entrez le prenom du patient : ";
			cin >> prenom;
			cout << "=> Entrez l'âge du patient     : ";
			cin >> age;
			cout << "=> Entrez le sexe du patient   : ";
			cin >> sexe;
			cout << "=> Entrez le CIN du patient    : ";
			cin >> CIN;
			ListPatients.AjouterPatient(nom, prenom, age, sexe, CIN);
			break;
		}
		case 2: {
			cout << "                 Afficher Patient" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			ListPatients.AfficherPatient();break;}
		case 3: {
			string CIN;
			cout << "                 Moddifier Patient" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez le CIN du patient à modifier : ";
			cin >> CIN;
			ListPatients.ModdifierPatient(CIN);
			break;
		}
		case 4: {
			string CIN;
			cout << "                 Supprimer Patient" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez le CIN du patient à supprimer : ";
			cin >> CIN;
			ListPatients.supprimerPatient(CIN);break;
		}
		case 5: {
			string type, date_vaccin, CIN;
			int Numero_vaccin;
			cout << "                  Ajouter Vaccin" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez le CIN du patient : ";
			cin >> CIN;
			cout << "=> Entrez le numero du vaccin : ";
			cin >> Numero_vaccin;
			cout << "=> Entrez le type du vaccin : ";
			cin >> type;
			cout << "=> Entrez la date du vaccination : ";
			cin >> date_vaccin;
			ListVaccins.AjouterVaccin(CIN, type, date_vaccin, Numero_vaccin);
			break;
		}
		case 6: {
			cout << "                 Afficher Vaccin" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			ListVaccins.AfficherVaccin();break;
		}
		case 7: {

			string CIN;
			cout << "                 Moddifier Vaccin" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez le CIN du patient à modifier : ";
			cin >> CIN;
			ListVaccins.ModdifierVaccin(CIN);break;
		}
		case 8: {
			string id;
			cout << "                 Supprimer Vaccin" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Entrez l'id du vaccination à supprimer : ";
			cin >> id;
			ListVaccins.SupprimerVaccin(id);break;
		}
		case 9: {
			cout << "                 Afficher STQ vaccin" << endl;
			cout << "           ############################     " << endl;
			cout << "                                           " << endl;
			cout << "=> Nombre de vaccinations    : " << sqlCon.GetNbrVaccinations() << endl;
			cout << "=> Nombre de premières doses : " << sqlCon.GetNbrPremiereDoses() << endl;
			cout << "=> Nombre de secondes doses  : " << sqlCon.GetNbrScndDose() << endl;
			break;
		}
		case 99: {
			sqlCon.Dispose();
			cout << "A bientôt :)" << endl;
			exit(1);
			break;
		}
		}
		cout << "###############################################" << endl;
		cout << "Voulez vous continuer? ->  y / n" << endl;
		cout << "###############################################" << endl;
		cin >> continu;
	} while (continu == "y");
	cout << "A bientôt :)" << endl;

	sqlCon.Dispose();
	system("pause");
	exit(1);

	return 0;
}