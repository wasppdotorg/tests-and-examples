# pgsqlpp

* sudo apt-get install postgresql postgresql-contrib libpq-dev pgadmin3

* sudo visudo
* user	ALL=(ALL) NOPASSWD:	ALL

* sudo nano /etc/postgresql/9.5/main/pg_hba.conf 
* local	all	postres	peer
<!--
* local	all	postres	trust
* local	all	postres	md5
-->

* sudo -i -u postgres

* psql
* ALTER USER postgres with encrypted password 'postgres';
* \q

* createuser --interactive
* createdb test1

* psql -h localhost -U test1 -W

