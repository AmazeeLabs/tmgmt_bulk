<?php

//AXXX
function tmgmt_bulk_form($form, &$form_state) {
//AXXX desc

  $languages = array();
  foreach (language_list() as $langcode => $language) {
    $languages[$langcode] = $language->name;
  }

  $label_inline = array(
    '#prefix' => '<div class="container-inline" style="margin: 0 0 10px;">',
    '#suffix' => '</div>',
  );

  $form['plugins'] = array(
    '#tree' => TRUE,
  );
  foreach (tmgmt_source_plugin_info() as $plugin_type => $plugin) {
    if (!empty($plugin['item types']) && ($selector = _tmgmt_bulk_get_selector($plugin_type))) {
      $form['plugins'][$plugin_type] = array(
        '#type' => 'fieldset',
        '#title' => $plugin['label'],
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['include'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include'),
      );
      $form['plugins'][$plugin_type]['item_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Item types'),
        '#states' => array(
          'visible' => array(
            'input[name="plugins[' . $plugin_type . '][include]"]' => array('checked' => TRUE),
          ),
        ),
        '#options' => $plugin['item types'],
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['filters'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filters'),
        '#states' => array(
          'visible' => array(
            'input[name="plugins[' . $plugin_type . '][include]"]' => array('checked' => TRUE),
          ),
        ),
        '#tree' => TRUE,
      );
      $form['plugins'][$plugin_type]['filters']['translated_to'] = array(
        '#type' => 'select',
        '#title' => t('Translated to'),
          //AXXX desc
        '#options' => array('' => '') + $languages,
      ) + $label_inline;
      $form['plugins'][$plugin_type]['filters']['not_exists_in_jobs'] = array(
        '#type' => 'select',
        '#title' => t('Does not exist in jobs with target language'),
          //AXXX desc
        '#options' => array('' => '') + $languages,
      ) + $label_inline;
      $form['plugins'][$plugin_type]['filters']['specific'] = array(
        '#tree' => TRUE,
      ) + $selector->getFormSpecificFilters();
    }
  }
  $form['actions'] = array('#type' => 'actions');
  //AXXX notify about long time??
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add to cart'),
  );

  return $form;
}

//AXXX
function tmgmt_bulk_form_submit(&$form, &$form_state) {
  /* @var $ret TMGMTBulkJobItem[] */
  $ret = array();
  $values = isset($form_state['values']['plugins']) ? $form_state['values']['plugins'] : array();
  //AXXX set timeout
  foreach ($values as $plugin => $state) {
    if ($state['include'] && ($item_types = array_filter($state['item_types']))) {
      $translated_to = $state['filters']['translated_to'];
      $not_exists_in_jobs = $state['filters']['not_exists_in_jobs'];
      $specific = isset($state['filters']['specific']) ? $state['filters']['specific'] : array();
      $ret = array_merge($ret, _tmgmt_bulk_get_selector($plugin)->searchItems($item_types, $translated_to, $not_exists_in_jobs, $specific));
    }
  }
  $i = 0;
//  foreach ($ret as $item) {
//    if (tmgmt_ui_cart_get()->addJobItem($item->plugin, $item->item_type, $item->item_id)) {
//      $i++;
//    }
//  }
  dpm(count($ret));
  drupal_set_message(format_plural($i, '1 item has been added to the cart.', '@count items have been added to the cart.'));
}
