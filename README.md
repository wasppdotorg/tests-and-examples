
Redmine
-------
* sudo su -
* apt update
* apt install mysql-server mysql-client libmysqlclient-dev imagemagick libmagickwand-dev libcurl4-openssl-dev git-core subversion

* gpg2 --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3

* apt install curl
* curl -L https://get.rvm.io | bash -s stable --ruby=2.2.5

* source /usr/local/rvm/scripts/rvm
* echo '[[ -s "/usr/local/rvm/scripts/rvm" ]] && source "/usr/local/rvm/scripts/rvm"' >> ~/.bashrc

* mysql -u root -p
* create database redmine;
* grant all privileges on redmine.* to redmine@localhost identified by 'redmine';
* flush privileges;

* gem install passenger --no-ri --no-rdoc
* passenger-install-nginx-module

* cd /opt/nginx/conf/
* vim nginx.conf
```
    server {
        listen       8000;
        server_name  localhost;

        root /var/www/redmine/public;
        passenger_enabled on;
        client_max_body_size      10m; # Max attachemnt size

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
```

* cd /lib/systemd/system/
* vim nginx.service
```
[Unit]
Description=The NGINX HTTP and reverse proxy server
After=syslog.target network.target remote-fs.target nss-lookup.target
 
[Service]
Type=forking
PIDFile=/opt/nginx/logs/nginx.pid
ExecStartPre=/opt/nginx/sbin/nginx -t
ExecStart=/opt/nginx/sbin/nginx
ExecReload=/bin/kill -s HUP $MAINPID
ExecStop=/bin/kill -s QUIT $MAINPID
PrivateTmp=true
 
[Install]
WantedBy=multi-user.target
```

* systemctl daemon-reload
* systemctl start nginx

* cd /var/www/
* svn co https://svn.redmine.org/redmine/branches/3.2-stable redmine

* cd redmine
* cp config/configuration.yml.example config/configuration.yml
* cp config/database.yml.example config/database.yml

* vim config/database.yml
```
production:
  adapter: mysql2
  database: redmine
  host: localhost
  username: redmine
  password: "redmine"
  encoding: utf8
```

* mkdir -p tmp tmp/pdf public/plugin_assets
* chown -R nobody:nogroup files log tmp public/plugin_assets
* chmod -R 775 files log tmp public/plugin_assets

* gem install bundler
* bundle install --without development test

* bundle exec rake generate_secret_token
* RAILS_ENV=production bundle exec rake db:migrate
* RAILS_ENV=production bundle exec rake redmine:load_default_data

* systemctl restart nginx

SVN
---
* sudo apt update
* sudo apt install subversion apache2 libapache2-svn apache2-utils
* sudo mkdir -p /var/www/svn/
* sudo svnadmin create /var/www/svn/test
* sudo chown -R nobody:nogroup /var/www/svn/test

* sudo vim /etc/sites-available/default-svn.conf
```
<Location /svn>
  DAV svn
  SVNParentPath /var/www/svn/
  AuthType Basic
  AuthName "Subversion Repositories"
  AuthUserFile /etc/apache2/dav_svn.users
  AuthzSVNAccessFile /etc/apache2/dav_svn.authz
  Require valid-user
 </Location>
```

* sudo htpasswd -cm /etc/apache2/dav_svn.users user1
* sudo vim /etc/apache2/dav_svn.authz
```
[groups]
test_group = user1
test2_group = user2

[test:/]
@test_group = rw

[test2:/]
@test2_group = rw
```

* sudo a2ensite default-svn
* sudo /etc/init.d/apache2 restart

