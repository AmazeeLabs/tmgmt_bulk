<?php

//AXXX
class TMGMTBulkSelector_locale extends TMGMTBulkSelector implements TMGMTBulkSelectorInterface {

  //AXXX
  public function searchItems(array $item_types, $translated_to, $not_exists_in_jobs, array $specific) {
    $ret = array();
    $query = db_select('locales_source', 'ls')
      ->condition('ls.textgroup', $item_types, 'IN')
      ->fields('ls', array('lid', 'textgroup'));

    if ($translated_to) {
      $translation_table_alias = $query->leftJoin(
        'locales_target',
        NULL,
        "%alias.lid = ls.lid AND %alias.language = :language",
        array(':language' => $translated_to)
      );
      $query->isNotNull("$translation_table_alias.lid");
      if (module_exists('l10n_update') && !empty($specific['only_custom'])) {
        module_load_include('inc', 'l10n_update');
        $query->condition("$translation_table_alias.l10n_status", L10N_UPDATE_STRING_CUSTOM);
      }
    }

    if ($not_exists_in_jobs) {
      $this->notExistsInJobs($query, 'locale', 'ls.textgroup', 'ls.lid', $not_exists_in_jobs);
    }

    foreach ($query->execute()->fetchAll() as $row) {
      $ret[] = new TMGMTBulkJobItem('locale', $row->textgroup, $row->lid);
    }
    return $ret;
  }

  //AXXX
  public function getFormSpecificFilters() {
    if (module_exists('l10n_update')) {
      return array(
        'only_custom' => array(
          '#type' => 'checkbox',
          '#title' => t('Include only custom strings'),
          //AXXX desc
          '#states' => array(
            'invisible' => array(
              'select[name="plugins[locale][filters][translated_to]"]' => array('value' => ''),
            ),
          ),
        ),
      );
    }
    return parent::getFormSpecificFilters();
  }
}
