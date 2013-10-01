=== SUMMARY ===
Jasperforge SSO Server provides single sign-on functionality to supported clients.
It is built as a front-end
for Drupal's CAS module.

=== REQUIREMENTS ===
CAS Server

=== INSTALLATION ===
Before installing this module, make sure CAS Server is properly installed first.

=== CONFIGURATION ===
Access the configuration page at admin/config/people/jf-sso
Make sure to generate a new secret key before using this module.
Enter any sites that you want to use with SSO Server.

=== USAGE ===
User management is done on the SSO Server. All users who don't already have an account
on the site that runs SSO Server will need to create a user account first.
The registration process has been customized to work with SSO service.