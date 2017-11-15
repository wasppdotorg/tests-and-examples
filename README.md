
SVN
---
* sudo apt update
* sudo apt install subversion apache2 libapache2-svn apache2-utils
* sudo mkdir -p /var/www/svn/
* sudo svnadmin create /var/www/svn/test
* sudo chown -R www-data:www-data /var/www/svn/test

* sudo vim /etc/apache2/sites-available/default-svn.conf
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

