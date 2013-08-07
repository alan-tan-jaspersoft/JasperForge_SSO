<?php
/**
 * @file
 * jasperforge_profile.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function jasperforge_profile_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'jasperforge_profile';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Community Profile';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: User */
  $handler = $view->new_display('panel_pane', 'User', 'user');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_type'] = 'h2';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_class'] = 'user-picture-large';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['link_photo_to_profile'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['points']['id'] = 'points';
  $handler->display->display_options['fields']['points']['table'] = 'userpoints_total';
  $handler->display->display_options['fields']['points']['field'] = 'points';
  $handler->display->display_options['fields']['points']['label'] = '';
  $handler->display->display_options['fields']['points']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['points']['alter']['text'] = '<div id="point-value">[points]</div>Reputation';
  $handler->display->display_options['fields']['points']['element_label_colon'] = FALSE;
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 'fields_override';
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'panel',
      'context' => 'entity:user.uid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: Uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Bio */
  $handler = $view->new_display('panel_pane', 'Bio', 'bio');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Bio';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'flipped';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_first_name' => 'field_last_name',
    'field_last_name' => 'field_last_name',
    'mail' => 'mail',
    'field_user_phone' => 'field_user_phone',
    'field_country' => 'field_country',
    'field_state_can' => 'field_state_can',
    'field_state_us' => 'field_state_us',
    'nothing_1' => 'field_country',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_first_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_last_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 1,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_user_phone' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_country' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ', ',
      'empty_column' => 1,
    ),
    'field_state_can' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_state_us' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => ',',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['flipped_table_header_first_field'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'User Name';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: First Name */
  $handler->display->display_options['fields']['field_first_name']['id'] = 'field_first_name';
  $handler->display->display_options['fields']['field_first_name']['table'] = 'field_data_field_first_name';
  $handler->display->display_options['fields']['field_first_name']['field'] = 'field_first_name';
  $handler->display->display_options['fields']['field_first_name']['label'] = '';
  $handler->display->display_options['fields']['field_first_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_first_name']['settings'] = array(
    'click_to_edit' => 1,
    'click_to_edit_style' => 'hover',
    'empty_text' => '',
    'fallback_format' => 'text_default',
  );
  /* Field: User: Last Name */
  $handler->display->display_options['fields']['field_last_name']['id'] = 'field_last_name';
  $handler->display->display_options['fields']['field_last_name']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['fields']['field_last_name']['field'] = 'field_last_name';
  $handler->display->display_options['fields']['field_last_name']['label'] = 'Real Name';
  $handler->display->display_options['fields']['field_last_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_last_name']['settings'] = array(
    'click_to_edit' => 1,
    'click_to_edit_style' => 'hover',
    'empty_text' => '',
    'fallback_format' => 'text_default',
  );
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['element_label_colon'] = FALSE;
  /* Field: User: Phone */
  $handler->display->display_options['fields']['field_user_phone']['id'] = 'field_user_phone';
  $handler->display->display_options['fields']['field_user_phone']['table'] = 'field_data_field_user_phone';
  $handler->display->display_options['fields']['field_user_phone']['field'] = 'field_user_phone';
  $handler->display->display_options['fields']['field_user_phone']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_phone']['settings'] = array(
    'click_to_edit' => 1,
    'click_to_edit_style' => 'hover',
    'empty_text' => '',
    'fallback_format' => 'phone',
  );
  /* Field: User: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['label'] = 'Location';
  $handler->display->display_options['fields']['field_country']['element_label_colon'] = FALSE;
  /* Field: User: Province */
  $handler->display->display_options['fields']['field_state_can']['id'] = 'field_state_can';
  $handler->display->display_options['fields']['field_state_can']['table'] = 'field_data_field_state_can';
  $handler->display->display_options['fields']['field_state_can']['field'] = 'field_state_can';
  $handler->display->display_options['fields']['field_state_can']['label'] = '';
  $handler->display->display_options['fields']['field_state_can']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_state_can']['element_label_colon'] = FALSE;
  /* Field: User: State */
  $handler->display->display_options['fields']['field_state_us']['id'] = 'field_state_us';
  $handler->display->display_options['fields']['field_state_us']['table'] = 'field_data_field_state_us';
  $handler->display->display_options['fields']['field_state_us']['field'] = 'field_state_us';
  $handler->display->display_options['fields']['field_state_us']['label'] = '';
  $handler->display->display_options['fields']['field_state_us']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_state_us']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'State/Province';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[field_state_us][field_state_can]';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 'fields_override';
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'panel',
      'context' => 'entity:comment.field-bug-attachments',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: Uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Company Info */
  $handler = $view->new_display('panel_pane', 'Company Info', 'company_info');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Company Info';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'flipped';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_title' => 'field_user_title',
    'field_user_role' => 'field_user_role',
    'field_company' => 'field_company',
    'field_company_size' => 'field_company_size',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_user_role' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_company' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_company_size' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  $handler->display->display_options['style_options']['flipped_table_header_first_field'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Title */
  $handler->display->display_options['fields']['field_user_title']['id'] = 'field_user_title';
  $handler->display->display_options['fields']['field_user_title']['table'] = 'field_data_field_user_title';
  $handler->display->display_options['fields']['field_user_title']['field'] = 'field_user_title';
  $handler->display->display_options['fields']['field_user_title']['element_label_colon'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_user_role']['id'] = 'field_user_role';
  $handler->display->display_options['fields']['field_user_role']['table'] = 'field_data_field_user_role';
  $handler->display->display_options['fields']['field_user_role']['field'] = 'field_user_role';
  /* Field: User: Company */
  $handler->display->display_options['fields']['field_company']['id'] = 'field_company';
  $handler->display->display_options['fields']['field_company']['table'] = 'field_data_field_company';
  $handler->display->display_options['fields']['field_company']['field'] = 'field_company';
  /* Field: User: Company Size */
  $handler->display->display_options['fields']['field_company_size']['id'] = 'field_company_size';
  $handler->display->display_options['fields']['field_company_size']['table'] = 'field_data_field_company_size';
  $handler->display->display_options['fields']['field_company_size']['field'] = 'field_company_size';
  $handler->display->display_options['fields']['field_company_size']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_company_size']['hide_empty'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'panel',
      'context' => 'entity:comment.field-bug-attachments',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: Uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Stats */
  $handler = $view->new_display('panel_pane', 'Stats', 'stats');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Stats';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'flipped';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid' => 'uid',
    'created' => 'created',
    'access' => 'access',
    'rid' => 'rid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'access' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  $handler->display->display_options['style_options']['flipped_table_header_first_field'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Member For';
  $handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
  /* Field: User: Last access */
  $handler->display->display_options['fields']['access']['id'] = 'access';
  $handler->display->display_options['fields']['access']['table'] = 'users';
  $handler->display->display_options['fields']['access']['field'] = 'access';
  $handler->display->display_options['fields']['access']['label'] = 'Last Access';
  $handler->display->display_options['fields']['access']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['access']['date_format'] = 'short';
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 'fields_override';
  $handler->display->display_options['argument_input'] = array(
    'uid' => array(
      'type' => 'panel',
      'context' => 'entity:comment.field-bug-attachments',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: Uid',
    ),
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: top-right-1 */
  $handler = $view->new_display('panel_pane', 'top-right-1', 'top_right_1');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: About Me */
  $handler->display->display_options['fields']['field_about_me']['id'] = 'field_about_me';
  $handler->display->display_options['fields']['field_about_me']['table'] = 'field_data_field_about_me';
  $handler->display->display_options['fields']['field_about_me']['field'] = 'field_about_me';
  $handler->display->display_options['fields']['field_about_me']['element_label_type'] = 'h3';
  $handler->display->display_options['fields']['field_about_me']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_about_me']['empty'] = 'Your <em>About Me</em> is empty.';
  /* Field: User: Signature */
  $handler->display->display_options['fields']['signature']['id'] = 'signature';
  $handler->display->display_options['fields']['signature']['table'] = 'users';
  $handler->display->display_options['fields']['signature']['field'] = 'signature';
  $handler->display->display_options['fields']['signature']['element_label_type'] = 'h3';
  $handler->display->display_options['fields']['signature']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['signature']['empty'] = 'You don\'t have a <em>Signature</em> yet.';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 0;
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 'fields_override';

  /* Display: Activity */
  $handler = $view->new_display('panel_pane', 'Activity', 'activity');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Activity';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'user/!1/track';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<em>Nothing Yet&hellip;</em>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: User: Content authored */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'users';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'uid';
  $handler->display->display_options['fields']['created']['label'] = 'Post Date';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['custom_date_format'] = '1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'uid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'uid';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['allow']['use_pager'] = 'use_pager';
  $handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 'more_link';
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['link_to_view'] = '0';

  /* Display: Social Network */
  $handler = $view->new_display('panel_pane', 'Social Network', 'social_network');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Social Network';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Social Network */
  $handler->display->display_options['fields']['field_social_network']['id'] = 'field_social_network';
  $handler->display->display_options['fields']['field_social_network']['table'] = 'field_data_field_social_network';
  $handler->display->display_options['fields']['field_social_network']['field'] = 'field_social_network';
  $handler->display->display_options['fields']['field_social_network']['label'] = '';
  $handler->display->display_options['fields']['field_social_network']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_social_network']['click_sort_column'] = 'first';
  $handler->display->display_options['fields']['field_social_network']['type'] = 'double_field_table';
  $handler->display->display_options['fields']['field_social_network']['settings'] = array(
    'table' => array(
      'number_column' => 0,
      'number_column_label' => '№',
      'first' => array(
        'column_label' => '',
      ),
      'second' => array(
        'column_label' => '',
      ),
    ),
    'first' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
    'second' => array(
      'hidden' => 0,
      'format' => '_none',
      'prefix' => '',
      'suffix' => '',
    ),
  );
  $handler->display->display_options['fields']['field_social_network']['delta_offset'] = '0';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $export['jasperforge_profile'] = $view;

  return $export;
}
